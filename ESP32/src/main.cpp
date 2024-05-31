#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#include "settings.h"
#include "functions.h"  // Hier wird das neue Headerfile eingebunden


//////////////////////////////////////////////////////////////////////////////
//
// Global verwendete Variabeln
//
//////////////////////////////////////////////////////////////////////////////

    int open_time[12][31] = {
        {1015,1015,1015,1015,1015,1015,1015,1015,1015,1015,1015,1015,1010,1010,1010,1010,1010,1010,1010,1005,1005,1005,1005,1005,1005,1000,1000,1000,1000,955,955},
        {953,952,950,949,947,946,945,943,942,940,939,937,936,934,932,931,929,927,926,924,922,920,919,917,915,913,911,910,908,908,908},
        {906,904,902,900,858,856,854,852,850,848,847,845,843,841,839,837,835,833,831,829,827,825,823,821,819,817,815,813,811,809,807},
        {800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800},
        {800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800},
        {800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800},
        {800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800},
        {800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800},
        {800,800,800,800,800,800,800,800,800,800,800,800,802,803,804,806,807,808,810,811,812,814,815,816,818,819,820,822,823,824,824},
        {826,827,828,830,831,833,834,835,837,838,840,841,842,844,845,847,848,849,851,852,854,855,857,858,900,901,903,904,906,907,909},
        {910,912,913,915,916,918,919,921,922,924,925,927,928,930,931,932,934,935,937,938,940,941,942,944,945,946,948,949,950,952,952},
        {953,954,955,956,957,958,1000,1001,1002,1003,1003,1004,1005,1006,1007,1007,1008,1009,1009,1010,1011,1011,1011,1012,1012,1012,1013,1013,1013,1013,1013}
    };

    int close_time[12][31] = {
        {1814,1815,1816,1847,1818,1819,1820,1822,1823,1824,1825,1826,1828,1829,1830,1832,1833,1834,1836,1837,1839,1840,1842,1843,1845,1846,1848,1849,1851,1852,1854},
        {1855,1857,1858,1900,1902,1903,1905,1906,1908,1909,1911,1912,1914,1916,1917,1919,1920,1922,1923,1925,1926,1928,1929,1931,1932,1934,1936,1937,1938,1938,1938},
        {1940,1941,1943,1944,1946,1947,1949,1950,1952,1953,1955,1956,1958,1959,2000,2002,2003,2005,2006,2008,2009,2010,2012,2013,2015,2016,2017,2019,2020,2022,2023},
        {2024,2026,2027,2029,2030,2031,2033,2034,2036,2037,2038,2040,2041,2043,2044,2045,2047,2048,2050,2051,2052,2054,2055,2056,2058,2059,2101,2102,2103,2105,2105},
        {2106,2107,2109,2110,2112,2113,2114,2116,2117,2118,2119,2121,2122,2123,2125,2126,2127,2128,2129,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142},
        {2143,2144,2145,2146,2147,2147,2148,2149,2149,2150,2151,2151,2152,2152,2153,2153,2153,2154,2154,2154,2154,2155,2155,2155,2155,2155,2155,2155,2155,2154,2154},
        {2154,2154,2154,2153,2153,2152,2152,2151,2151,2150,2149,2149,2148,2147,2146,2146,2145,2144,2143,2142,2141,2140,2139,2137,2136,2135,2134,2133,2131,2130,2129},
        {2127,2126,2124,2123,2121,2120,2118,2117,2115,2114,2112,2110,2109,2107,2105,2103,2102,2100,2058,2056,2055,2053,2051,2049,2047,2045,2043,2041,2040,2038,2036},
        {2044,2042,2040,2028,2026,2024,2022,2020,2018,2016,2014,2012,2010,2008,2006,2004,2002,1959,1957,1955,1953,1951,1949,1947,1945,1943,1941,1939,1937,1935,1935},
        {1933,1931,1929,1927,1925,1923,1921,1919,1917,1915,1914,1912,1910,1908,1906,1904,1902,1901,1859,1857,1855,1853,1852,1850,1848,1847,1845,1843,1842,1840,1839},
        {1837,1835,1834,1833,1831,1830,1828,1827,1826,1824,1823,1822,1821,1820,1818,1817,1816,1815,1814,1813,1813,1812,1811,1810,1809,1809,1808,1807,1807,1806,1806},
        {1806,1806,1805,1805,1805,1804,1804,1804,1804,1804,1804,1804,1804,1804,1804,1805,1805,1805,1806,1806,1807,1807,1808,1808,1809,1810,1811,1811,1812,1813,1814}
    };

int minute = 0;                 
int hour = 0;
int day = 0;
int month = 0;
int year = 0;

int s_door_automatic;           
int s_door_override;
int s_back_to_auto;
int s_day;
int s_month;
int s_year;
int s_hours;
int s_minutes;
int s_door;

int warningTable = 0;           // Erstellen von Warnungs und Errortabelle
int errorTable = 0;

int systemstatus = 0;           
int tuerstatus = 0;
float aussentemperatur = 0.0;
float innentemperatur = 0.0;
int luefterstatus = 0;
float wassertemperatur = 0.0;
int lichtwert = 0;
int torstatus = 0;
int neststatus = 0;
float batteriespannung = 0.0;

int torbedingung = 0;

int sendFlag = 0;

Ticker timer; 

OneWire oneWire(S_TEMPERATUR);  
DallasTemperature sensors(&oneWire);    
DeviceAddress sensorAddresses[NUM_SENSORS];

//////////////////////////////////////////////////////////////////////////////
//
// Funktion: setup() initialisiert alle Verbindungen und sensoren
//
//////////////////////////////////////////////////////////////////////////////

void setup() {

    // Serielle Schnittstelle initialisieren
    #ifdef DEBUG
        Serial.begin(115200);
    #endif

    // WIFI Configuration
    const char* ssid = "KellerWlan";        
    const char* password = "77Internet77";

    sensors.begin(); // Temperatursensoren initialisieren
 
    delay(1000);   // delay für Sensoren

    pinMode(S_SET, INPUT); // Initialisieren der Hardware Inputs
    pinMode(U_LICHT, INPUT);
    pinMode(U_BAT, INPUT);
    pinMode(S_TOR_ZU, INPUT);
    pinMode(S_TOR_AUF, INPUT);
    pinMode(S_NOTAUSGANG, INPUT);
    pinMode(S_NEST3, INPUT);
    pinMode(S_NEST4, INPUT);
    pinMode(S_NEST1, INPUT);
    pinMode(S_NEST2, INPUT);
    
    pinMode(O_LICHT, OUTPUT);   // Initialisieren der Hardware Outputs
    pinMode(O_LED_ERROR, OUTPUT);
    pinMode(O_LED_WARNING, OUTPUT);
    pinMode(O_LED_OK, OUTPUT);
    pinMode(O_TOR_DOWN, OUTPUT);
    pinMode(O_LUEFTER, OUTPUT);
    pinMode(O_TOR_UP, OUTPUT);
    pinMode(E_LUEFTER, OUTPUT);
    pinMode(E_TOR, OUTPUT);
    pinMode(E_NEST, OUTPUT);

    digitalWrite(O_LICHT, HIGH);

    // OneWire verbindung aufbauen für Temperatursensoren
    sensors.getAddress(sensorAddresses[0], 0); // Adressen auslesen Sensor 0
    sensors.getAddress(sensorAddresses[1], 1); // Adressen auslesen Sensor 1
    sensors.getAddress(sensorAddresses[2], 2); // Adressen auslesen Sensor 2
    sensors.setResolution(sensorAddresses[0], 12); // Auflösung für Sensor 0 auf 12 Bit
    sensors.setResolution(sensorAddresses[1], 12); // Auflösung für Sensor 1 auf 12 Bit
    sensors.setResolution(sensorAddresses[2], 12); // Auflösung für Sensor 2 auf 12 Bit

    // Initialisiere die I2C-Schnittstelle
    Wire.begin(21, 22); // SDA an GPIO21, SCL an GPIO22

    // Funktion callFunction wird alle 5 Minuten aufgerufen mit einem Interrupt
    timer.attach(300.0, callFunction); 

    // WIFI-Verbindung aufbauen
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        #ifdef DEBUG
            Serial.println("Connecting to WiFi...");
        #endif
    }
    #ifdef DEBUG
        Serial.println("Connected to WiFi");
    #endif
}

//////////////////////////////////////////////////////////////////////////////
//
// Funktion: loop() Programmroutine.
//
//////////////////////////////////////////////////////////////////////////////

void loop() {
    
    getSettings();          // Systemsettings vom Webserver auslesen
    getTemperatureData();   // Alle Temperatursensoren auslesen
    readTime();             // Systemzeit aus RTC auslesen
    getAnalogData();        // Lichtsensor und Batteriespannung auslesen
    setLuefter();           // Schaltet Lüfter je nach Temperaturunterschied von Innen und Aussentemperatur
    getNestStatus();        // Liest den Status der Legenester aus
    setLicht();             // Schaltet das Licht je nach Bedarf ein oder aus
    
    if(digitalRead(S_NOTAUSGANG)){  // Notausgang auswerten und Status bzw. Warnung setzen
        warningTable &= ~(1<<WARNING_EXIT);
        tuerstatus = CLOSE;
    }
    else{
        warningTable |= (1<<WARNING_EXIT);
        tuerstatus = OPEN;
    }

    if (digitalRead(S_SET)){   // Abfrage ob Taster auf dem Board gedrückt wurde um eingestellte Zeit zu setzten
        setDS1307DateTime(s_day, s_month, s_year, s_hours, s_minutes, 0); // Setze das aktuelle Datum und die Zeit auf dem DS1307
    }

    int timeBCD = minute + (hour * 100); // Umrechnung von aktueller Zeit in BCD für Tabellenauswertung.
    
    if(timeBCD >= open_time[month-1][day-1] && timeBCD <= close_time[month-1][day-1] && lichtwert >= LICHT_TRIGGER_TUER){      // Wenn die aktuelle Zeit zwischen der Öffnungs und Schliesszeit liegt und der Lichtwert genügend hoch ist Türe öffnen

        torbedingung = OPEN;

    }
    else if (timeBCD <= open_time[month-1][day-1] || timeBCD >= close_time[month-1][day-1]){ // Wenn die aktuelle Zeit nach der Schliesszeit oder vor der öffnungszeit ist Torbedingung auf schliessen

        torbedingung = CLOSE;

    }
    if(s_door_automatic == AUS || s_door_override == EIN){  // Wenn die Türautomatik ausgestellt ist oder Türüberschreibung aktiv ist

        setDoor(s_door); // Setze Tor auf den im Server eingestellten Zustand.

    }
    else{
        setDoor(torbedingung);  // Im Automatikbetrieb das Tor nach Torbedingung setzen
    }
    if(s_door_automatic == AUS || s_door_override == 1 ){   // Überprüfen ob Torautomatik aus oder Torüberschreibung aktiv ist
        if(s_back_to_auto == EIN && torbedingung == s_door){    // Überprüfen ob die Einstellung Back to Auto aktiv und die Torbedingung gleich wie die Servereinstellung vom Server ist.
            insertSettings();   // Einstellungen vom Server überschreiben, da bedingung für Automatikbetrieb wieder erfüllt
        }
    }


    if(warningTable == 0 && errorTable == 0){   // Systemstatus auswerten
        systemstatus = STATUS_OK;
    }
    else if(warningTable >= 0 && errorTable == 0){
        systemstatus = STATUS_WARNING;
    }
    else{
        systemstatus = STATUS_ERROR;
    }
    switch (systemstatus){  //Systemstatus ausgeben per Signalleuchte
        case STATUS_OK:
            digitalWrite(O_LED_ERROR, LOW);
            digitalWrite(O_LED_WARNING, LOW);
            digitalWrite(O_LED_OK, HIGH);
        break;
        case STATUS_WARNING:
            digitalWrite(O_LED_ERROR, LOW);
            digitalWrite(O_LED_WARNING, HIGH);
            digitalWrite(O_LED_OK, LOW);
        break;
        case STATUS_ERROR:
            digitalWrite(O_LED_ERROR, HIGH);
            digitalWrite(O_LED_WARNING, LOW);
            digitalWrite(O_LED_OK, LOW);
        break;
    }
    #ifdef DEBUG
        Serial.print("Error codes: ");
        Serial.println(errorTable,BIN);
        Serial.print("Warning codes: ");
        Serial.println(warningTable,BIN);
        Serial.println(torbedingung);
        Serial.println(s_door);
        Serial.println(s_back_to_auto);
        Serial.println(s_door_override);
        Serial.println(s_door_automatic);
        Serial.println(timeBCD);
    #endif

    delay(9000); // Warte 9 Sekunden, bevor Sensoren erneut ausgelesen werden.

}