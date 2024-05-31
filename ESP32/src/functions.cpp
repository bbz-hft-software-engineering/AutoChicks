#include "functions.h"
#include "settings.h"
#include <Wire.h>
#include <OneWire.h>

extern DallasTemperature sensors;
extern DeviceAddress sensorAddresses[];
extern int minute, hour, day, month, year;
extern int s_door_automatic, s_door_override, s_back_to_auto, s_day, s_month, s_year, s_hours, s_minutes, s_door;
extern int systemstatus, tuerstatus, luefterstatus, torstatus, neststatus;
extern float aussentemperatur, innentemperatur, wassertemperatur, batteriespannung;
extern int lichtwert, torbedingung, sendFlag;
extern int warningTable, errorTable;
extern int open_time[12][31], close_time[12][31];

// Implementierungen der Funktionen
float readTemperature(DeviceAddress address) {
    sensors.requestTemperaturesByAddress(address);
    return sensors.getTempC(address);
}

int bcdToDec(int val) {
    return ( (val/16*10) + (val%16) );
}

byte decToBcd(byte val) {
    return ((val / 10 * 16) + (val % 10));
}

void setDS1307DateTime(int day, int month, int year, int hour, int minute, int second) {
    // Setze das Datum und die Zeit auf dem DS1307
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0); // Startadresse für die Datenübertragung

    // Sende das Datum und die Zeit
    Wire.write(decToBcd(second)); // Sekunde
    Wire.write(decToBcd(minute)); // Minute
    Wire.write(decToBcd(hour));   // Stunde
    Wire.write(decToBcd(0));      // Wochentag (wird nicht verwendet)
    Wire.write(decToBcd(day));    // Tag
    Wire.write(decToBcd(month));  // Monat
    Wire.write(decToBcd(year - 2000)); // Jahr (2-stellig, abgezogen von 2000)

    Wire.endTransmission();
    #ifdef DEBUG
        Serial.println("Datum und Zeit auf DS1307 gesetzt.");
    #endif
}

void callFunction() {
    #ifdef DEBUG
        Serial.println("Timer-Interrupt aufgerufen! Date werden an Webserver übertragen.");
    #endif
    
    HTTPClient http; // Verbindung zum Webserver herstellen

    // URL zur PHP-Datei auf dem Webserver
    String url = "http://192.168.31.102/insert_data.php";

    // Aufbau der Daten für den POST-Antrag
    String postData = "Systemstatus= " + String(systemstatus) +
                      "&Tuerstatus= " + String(tuerstatus) +
                      "&Aussentemperatur= " + String(aussentemperatur) +
                      "&Innentemperatur= " + String(innentemperatur) +
                      "&Luefterstatus= " + String(luefterstatus) +
                      "&Wassertemperatur= " + String(wassertemperatur) +
                      "&Lichtwert= " + String(lichtwert) +
                      "&Torstatus= " + String(torstatus) +
                      "&Neststatus= " + String(neststatus) +
                      "&Batteriespannung= " + String(batteriespannung) +
                      "&Eintragszeit= " +  String(hour)+"."+String(minute)+
                      "&Errortable= " + String(errorTable) +
                      "&Warningtable= " + String(warningTable);

    
    http.begin(url); // POST-Antrag senden
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postData);

    if (httpCode > 0) { // Überprüfen, ob der POST-Antrag erfolgreich war

        #ifdef DEBUG
            Serial.printf("[HTTP] POST-Antrag erfolgreich, HTTP-Statuscode: %d\n", httpCode);
        #endif
        warningTable &= ~(1 << WARNING_SEND_DATA);
        errorTable &= ~(1 << ERROR_SEND_DATA);
        sendFlag = 0;

    } else {
        
        #ifdef DEBUG
            Serial.printf("[HTTP] POST-Antrag fehlgeschlagen, HTTP-Statuscode: %d\n", httpCode);
        #endif
        if (sendFlag == 0){ // Im Fehlerfall Warnung oder Error setzen
            warningTable |= (1<<WARNING_SEND_DATA);
            sendFlag = 1;
        }
        else{
            errorTable |= (1 << ERROR_SEND_DATA);
        }
    }

    // Antwort des Servers lesen
    String payload = http.getString();
    #ifdef DEBUG
        Serial.println("Serverantwort: " + payload);
    #endif

    http.end(); // Verbindung schließen
}

void readTime(){
    // Lese das Datum und die Zeit vom DS1307 aus
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0); // Startadresse für die Datenübertragung
    Wire.endTransmission();
  
    // Fordere 7 Bytes (Sekunde, Minute, Stunde, Wochentag, Tag, Monat, Jahr)
    Wire.requestFrom(DS1307_ADDRESS, 7);
  
    // Überprüfe, ob genügend Daten empfangen wurden
    if (Wire.available()) {
        int second = bcdToDec(Wire.read() & 0x7F); // Ignoriere das höchstwertige Bit
        minute = bcdToDec(Wire.read());
        hour = bcdToDec(Wire.read() & 0x3F); // Ignoriere das höchstwertige Bit (12-Stunden-Modus)
        int weekday = bcdToDec(Wire.read());
        day = bcdToDec(Wire.read());
        month = bcdToDec(Wire.read());
        year = bcdToDec(Wire.read());
        year += 2000;

        if(year <= 2023){ // Prüfen ob der RTC die Zeiteinstellungen verloren hat und Status setzen
            errorTable |= (1<<ERROR_DATE);
        }
        else{
            errorTable &= ~(1<<ERROR_DATE);
        }

        #ifdef DEBUG
        // Ausgabe des Datums und der Zeit auf der seriellen Schnittstelle
            Serial.print("Datum: ");
            Serial.print(day);
            Serial.print(".");
            Serial.print(month);
            Serial.print(".");
            Serial.print(year);
            Serial.print("  Zeit: ");
            Serial.print(hour);
            Serial.print(":");
            if (minute < 10) {
                Serial.print("0"); // Führende Null hinzufügen, wenn die Minuten kleiner als 10 sind
            }
            Serial.print(minute);
            Serial.print(":");
            if (second < 10) {
                Serial.print("0"); // Führende Null hinzufügen, wenn die Sekunden kleiner als 10 sind
            }
            Serial.print(second);
            Serial.println();
        #endif
    }
}

void getSettings(){

    const char* serverUrl = "http://192.168.31.102/get_latest_settings.php";

     if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            
            StaticJsonDocument<512> doc; // JSON-Daten analysieren
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {   // Wenn kein Error vorhanden Daten übernehmen
                s_door_automatic = doc["door_automatic"];
                s_door_override = doc["door_override"];
                s_back_to_auto = doc["back_to_auto"];
                s_day = doc["day"];
                s_month = doc["month"];
                s_year = doc["year"];
                s_hours = doc["hours"];
                s_minutes = doc["minutes"];
                s_door = doc["door"];

                warningTable &= ~(1 << WARNING_GET_SETTINGS);

            } 
            else {
                warningTable |= (1 << WARNING_GET_SETTINGS);
                #ifdef DEBUG
                    Serial.println("Fehler beim Parsen der JSON-Daten");
                #endif
            }
        } 
        else {
            warningTable |= (1 << WARNING_GET_SETTINGS);
            #ifdef DEBUG
                Serial.println("Fehler bei der HTTP-Anfrage");
            #endif
        }

        http.end();
    }
    else{
         warningTable |= (1 << WARNING_GET_SETTINGS);
        #ifdef DEBUG
            Serial.println("Fehler bei der HTTP-Anfrage");
        #endif
    }
}

void getAnalogData(){
    lichtwert = analogRead(U_LICHT);                        // Lichtsensor auswerten
    int sensorValue = analogRead(U_BAT);                    // Batteriespannung des Akkus messen
    batteriespannung = sensorValue * (14.4 / 4095);         // Umrechnen in die Spannung

    if (batteriespannung > 12){                             // Batteriespannung prüfen und den dazugehörigen Status setzen
        warningTable &= ~(1 << WARNING_BAT);
        errorTable &= ~(1 << ERROR_BAT);
    }
    else if(batteriespannung <= 12 && batteriespannung > 11){
        warningTable |= (1 << WARNING_BAT);
        errorTable &= ~(1 << ERROR_BAT);
    }
    else{
        warningTable &= ~(1 << WARNING_BAT);
        errorTable |= (1 << ERROR_BAT);
    }

    #ifdef DEBUG
        Serial.print("Batteriespannung: ");
        Serial.print(batteriespannung, 2); // Zeige die Spannung auf zwei Dezimalstellen genau
        Serial.println(" V");
        Serial.print("Lichtwert: ");
        Serial.println(lichtwert); // Zeige die Spannung auf zwei Dezimalstellen genau
    #endif
    
}

void getTemperatureData(){

    for (int i = 0; i < NUM_SENSORS; i++) {
        float tempC = readTemperature(sensorAddresses[i]);
        
        #ifdef DEBUG
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(" Temperatur: ");
            Serial.print(tempC);
            Serial.println("°C");
        #endif

        switch(i){
            case WASSERTEMP:
            wassertemperatur = tempC;
            break;

            case RAUMTEMP:
            innentemperatur = tempC;
            break;

            case AUSSENTEMP:
            aussentemperatur = tempC;
            break;
        };
    }

    if(wassertemperatur <= 0){ // Wasertemperatur auf möglichen Frost prüfen und Status setzen
        warningTable |= (1 << WARNING_WATER_TEMP);
    }
    else{
        warningTable &= ~(1 << WARNING_WATER_TEMP);
    }

    if(innentemperatur >= 35 || innentemperatur <= -10){ // Innentemperatur auf extreme prüfen und Status setzen
        warningTable |= (1 << WARNING_AIR_TEMP);
    }
    else{
        warningTable &= ~(1 << WARNING_AIR_TEMP);
    }
}

void setLuefter(){

    if(batteriespannung >= 12){  // Überprüfen ob Batterie genug Kapazität aufweist um Lüfter zu betreiben
        if((innentemperatur-aussentemperatur)>= 2 && innentemperatur >=27){ // Schalte Lüfter ein wenn innenTemperatur 2°C höher ist als aussentemperatur
            digitalWrite(E_LUEFTER, HIGH);
            digitalWrite(O_LUEFTER, HIGH);
        }
        else if((innentemperatur-aussentemperatur)<= 1){ // Schalte Lüfter aus, wenn der Temperaturunterschied kleiner 1°C ist.
            digitalWrite(E_LUEFTER, LOW);
            digitalWrite(O_LUEFTER, LOW);            
        }
    }
}

void getNestStatus(){

    digitalWrite(E_NEST, HIGH);             // Distanzsensoren aktivieren
    delay(500);                             // Kurze Wartezeit zum initialisieren
    int state1 = digitalRead(S_NEST1);      // Sensoren auslesen
    int state2 = digitalRead(S_NEST2);
    int state3 = digitalRead(S_NEST3);
    int state4 = digitalRead(S_NEST4);

    if (state1 == LOW || state2 == LOW || state3 == LOW || state4 == LOW) { // Abfrage ob sensor ausgelöst wurde
        neststatus = 1; // Status setzten
        if(hour >= close_time[month][day]){     // Wenn Nest besetzt nach Türschliessung warnung setzten
            warningTable |= (1<<WARNING_NEST);
        }
    } else {
        neststatus = 0;
        warningTable &= ~(1<<WARNING_NEST);
    }
    digitalWrite(E_NEST, LOW);                  // Distanzsensoren deaktivieren
}

void setLicht(){

    if(batteriespannung >= 12){             // Überprüfen ob Batterie genug Kapazität aufweist um Licht zu betreiben
        if ((hour >=LICHT_EINSCHALTZEIT) && (hour <=LICHT_AUSSCHALTZEIT) && (lichtwert <= LICHT_TRIGGER_LED)){  // Überprüfen ob die Zeit und Lichtintensität zu den einstellungen passt.
            digitalWrite(O_LICHT, LOW);     // Licht einschalten
        }
        else{
            digitalWrite(O_LICHT, HIGH);    // Licht ausschalten, wenn eine der Bedingungen nicht mehr erfüllt ist
        }
    }
    else{
        digitalWrite(O_LICHT, HIGH);        // Licht ausschalten, wenn Batteriespannung zu gering ist
    }
}

void setDoor(int direction){
    
    int timer = 0;  // Timer Variable zum messen maximal erlaubter Öffen- und Schliesszeiten
    if(direction == OPEN && digitalRead(S_TOR_ZU)){ // Überprüfen ob Tor geöffnet werden soll und das Tor bereits geschlossen ist

        digitalWrite(E_TOR, HIGH);  // Motor aktivieren
        digitalWrite(O_TOR_UP, HIGH);
        while(digitalRead(S_TOR_AUF) == 0 && timer <= DOOR_TIME){    // Warten bis tor in Endposition oder Maximale Zeit überschritten
            timer += 1;
            delay(1);
        }
        digitalWrite(E_TOR, LOW);   // Motor deaktivieren
        digitalWrite(O_TOR_UP, LOW);
    }
    if(direction == CLOSE && digitalRead(S_TOR_AUF)){ // Überprüfen ob Tor geschlossen werden soll und Tor geöffnet ist.

        digitalWrite(E_TOR, HIGH);  // Motor aktivieren
        digitalWrite(O_TOR_DOWN, HIGH);
        while(digitalRead(S_TOR_ZU) == 0 && timer <= DOOR_TIME){    // Warten bis tor in Endposition oder Maximale Zeit überschritten
            timer += 1;
            delay(1);
        }
        digitalWrite(E_TOR, LOW);   // Motor deaktivieren
        digitalWrite(O_TOR_DOWN, LOW);
    }

    if(digitalRead(S_TOR_AUF) && !digitalRead(S_TOR_ZU)){       // Torstatus bestimmen
         torstatus = OPEN;
         if(torbedingung == OPEN || s_door_override == EIN){    // Fehlererkennung
            errorTable &= ~(1<<ERROR_DOOR);
         }
         else{
            errorTable |= (1<<ERROR_DOOR);
         }
         
    }
    else if(!digitalRead(S_TOR_AUF) && digitalRead(S_TOR_ZU)){  // Torstatus bestimmen
        torstatus = CLOSED;
        if(torbedingung == CLOSED || s_door_override == EIN){   // Fehlererkennung
            errorTable &= ~(1<<ERROR_DOOR);
         }
         else{
            errorTable |= (1<<ERROR_DOOR);
         }
    }
    else{
        torstatus = OPEN;
        errorTable |= (1<<ERROR_DOOR);
    }
}

void insertSettings(void){
    
    HTTPClient http;

    // URL zur PHP-Datei auf dem Webserver
    String urlSettings = "http://192.168.31.102/insert_settings.php";

    // Aufbau der Daten für den POST-Antrag
    String postSettings =   "door_automatic= " + String(EIN) +
                            "&door_override= " + String(AUS) +
                            "&back_to_auto= " + String(EIN) +
                            "&day= " + String(day) +
                            "&month= " + String(month) +
                            "&year= " + String(year) +
                            "&hours= " + String(hour) +
                            "&minutes= " + String(minute) +
                            "&door= " + String(CLOSE);

    http.begin(urlSettings);    // POST-Antrag senden
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postSettings);

    if (httpCode > 0) { // Überprüfen, ob der POST-Antrag erfolgreich war

        #ifdef DEBUG
            Serial.printf("[HTTP] POST-Antrag erfolgreich, HTTP-Statuscode: %d\n", httpCode);
        #endif
        warningTable &= ~(1 << WARNING_SEND_DATA);      // Fehlercodes zurücksetzten bei erfolgreichen schreiben
        errorTable &= ~(1 << ERROR_SEND_DATA);
        sendFlag = 0;

    } else {
        
        #ifdef DEBUG
            Serial.printf("[HTTP] POST-Antrag fehlgeschlagen, HTTP-Statuscode: %d\n", httpCode);
        #endif

        if (sendFlag == 0){     // Warungsausgabe nach einer Fehlgeschlagenen übertragung und Error bei zwei aufeinanderfolgenden
            warningTable |= (1<<WARNING_SEND_DATA);
            sendFlag = 1;
        }
        else{
            errorTable |= (1 << ERROR_SEND_DATA);
        }
    }

    String payload = http.getString();    // Antwort des Servers lesen
    #ifdef DEBUG
        Serial.println("Serverantwort: " + payload);
    #endif

    http.end();     // Verbindung schließen
}