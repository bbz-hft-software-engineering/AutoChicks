//////////////////////////////////////////////////////////////////////////////
//
// Software: ESP32 Automatisierter Hühnerstall
// Funktion: Einstellungen der Hardware können hier vorgenommen werden
// Autor:    Eric Suter
// Datum:    12.05.2024
//
//////////////////////////////////////////////////////////////////////////////

#define DEBUG                   // Serielle Ausgabe für Debugging aktivieren oder deaktivieren

//////////////////////////////////////////////////////////////////////////////
//
// Pinkonfiguration Einstellungen am ESP 32
//
//////////////////////////////////////////////////////////////////////////////

#define U_LICHT 36 
#define U_BAT 39
#define S_NEST1 34
#define S_NEST2 35
#define S_NEST3 32
#define S_NEST4 33
#define E_NEST 25
#define S_NOTAUSGANG 26
#define S_TEMPERATUR 27
#define S_TOR_AUF 14
#define S_TOR_ZU 12
#define E_TOR 23
#define E_LUEFTER 19
#define O_TOR_UP 18
#define O_TOR_DOWN 5
#define O_LUEFTER 17
#define O_LED_OK 16
#define O_LED_WARNING 4
#define O_LED_ERROR 0
#define O_LICHT 2
#define S_SET 13

//////////////////////////////////////////////////////////////////////////////
//
// Sensorkonfiguration und Fine Tuning
//
//////////////////////////////////////////////////////////////////////////////
#define LICHT_EINSCHALTZEIT 7
#define LICHT_AUSSCHALTZEIT 9   
#define LICHT_TRIGGER_TUER 2000
#define LICHT_TRIGGER_LED  1500
#define DOOR_TIME 25000 // Maximale schliess und Öffnungszeit für das Toor in ms

#define NEST_CLOSE_ZEIT     18


#define NUM_SENSORS 3           // Anzahl der angeschlossenen Temperatursensoren

#define DS1307_ADDRESS 0x68     // I2C-Adresse des DS1307

//////////////////////////////////////////////////////////////////////////////
//
// ERROR Handling
//
//////////////////////////////////////////////////////////////////////////////

// Fehlertabelle für Systemanalyse
#define ERROR_BAT               0
#define ERROR_DOOR              1
#define ERROR_DATE              2
#define ERROR_SEND_DATA         3

#define WARNING_BAT             0
#define WARNING_GET_SETTINGS    1
#define WARNING_SEND_DATA       2
#define WARNING_WATER_TEMP      3
#define WARNING_AIR_TEMP        4
#define WARNING_NEST            5
#define WARNING_EXIT            6

//////////////////////////////////////////////////////////////////////////////
//
// Defines für lesbareren Code
//
//////////////////////////////////////////////////////////////////////////////

#define WASSERTEMP 0
#define RAUMTEMP 1
#define AUSSENTEMP 2

#define STATUS_OK 0
#define STATUS_WARNING 1
#define STATUS_ERROR 2

#define OPEN 1
#define CLOSED 0
#define CLOSE 0
#define EIN 1
#define AUS 0

#define PASS 1
#define FAIL 0
