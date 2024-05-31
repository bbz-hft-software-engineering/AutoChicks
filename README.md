# AutoChicks

##	Aufgabenanalyse
### Feststellung des Ist-Zustandes
Zum Zeitpunkt des Starts des Projektes ist ein konventioneller Hühnerstall aus Holz mit sechs Quadratmetern Fläche vorhanden. Um die Hühner in der Nacht vor Fressfeinden zu schützen, muss dieser Stall abends während der Dämmerung geschlossen und morgens nach Sonnenaufgang geöffnet werden. 
Der Stall ist während der Sommermonate starker Sonneneinstrahlung ausgesetzt, was eine erhöhte Innentemperatur zur Folge hat. Da Hühner nicht schwitzen können, muss durch Öffnen und Schliessen der Fenster ein für Hühner angemessenes Klima eingestellt werden.
Währen der Wintermonate friert die Tränke der Hühner bei kalter Witterung zu. Um dem entgegenzuwirken, muss an frostigen Nächten und Tagen die Tränke mehrfach kontrolliert und bei Bedarf mit warmem Wasser befüllt werden. 
Da die Nahrungsaufnahme der Tiere nur mit genügend Licht möglich ist, reduzieren diese die Eierproduktion während der kurzen Wintertage, oder stellen diese teilweise gänzlich ein, bis die Tage im Frühling wieder länger werden.
An manchen Tagen kommt es vor, dass einzelne Hühner später in den Stall gehen und dadurch den Weg auf die Schlafstangen nicht mehr finden. Diese Hühner begeben sich dann in die Legenester, um dort die Nacht zu verbringen, was ein Problem für die Hygiene ist, da der Kot in den Legenester abgesetzt wird. 

 
###	Problemanalyse
Aus dem Ist-Zustand ist zu erkennen, dass täglich ein grosser Zeitaufwand entsteht, welcher unter Umständen nicht mit dem Alltag des Diplomanden vereinbar ist. An solchen Tagen können die Tiere aus Sicherheitsgründen den Stall nicht verlassen, da die Gefahr von Fressfeinen in der Nacht besteht, wenn die Zugangsklappe nicht zur richtigen Zeit geöffnet oder geschlossen wird.
Die Automatisierung und Überwachung der Kritischen Infrastruktur wie der Zugangstüre der Tiere, Wasserversorgung und Belüftung kann deshalb den Überwachungsaufwand reduzieren und das Tierwohl steigern, indem diese öfter den Zugang zum Freilauf bekommen. 
Das Tierwohl und die Sicherheit muss bei der Planung, der Durchführung und im Betrieb des Projekts stets gewahrt werden. 

 
##	Pflichtenheft
###	Mindestanforderungen
Im Rahmen des Softwareprojekts soll die Planung und Umsetzung der Software stattfinden. 

Folgende Funktionen müssen erfüllt werden:
-	Automatisierte Türsteuerung gemäss Zeit und Dämmerungssensor
-	Raum- und Aussentemperaturmessung 
-	Lüftungsfunktion gemäss Temperaturunterschied
-	Mechanisch betätigter Notausgang
-	Trinkwassertemperatur-Überwachung
-	Automatische Lichtsteuerung zur Tagesverlängerung
-	Über Signalleuchten wird der Status des Systems angezeigt
-	Überwachung der Belegung der Legenester
-	Anbindung des Systems an das Internet zur Überwachung bei Abwesenheit

Folgende Nicht Funktionale anforderungen müssen beachtet werden:
-	Die Umsetzung hat auf einem ESP32 zu erfolgen, welcher in der Programmiersprache C programmiert werden muss.
-	Der Webserver läuft auf einem Raspberry Pi.
-	Die Webandwendung läuft auf PHP
-	Es ist eine MYSQL Datenbank zum Austausch von Daten zu verwenden

## Systemdesign

TODO

## Coding

Der Code wurde in der IDE Visual Studio Code mit PlatformIO geschrieben.
Es werden diverse Librarys benötigt, welche im Code ersichtlich sind. Die installation dieser findet über den Library HUB von PlatformIO statt.

Der compilierte CODE wird per USB auf das Developmentboard des ESP32 geladen. Ein entsprechender Treiber für das Entwicklungsboard muss vorläufig vom Hersteller des Chips bezogen und installiert werden.


## Testing

Das Testing findet per ausgabe auf dem Seriellen-Monitor der IDE statt. Wichtige Daten und Systemzustände werden darüber ausgegeben, insofern die Option im settings.h aktiviert wurde.
Mit einem ausführlichen Testprotokoll ( Siehe Exel Tabelle ) wurde die Korrekte funktion der Software getrackt und dokumentiert. 
Hochautomatisierte Unit tests gestalten sich schwierig, da der komplette Softwareablauf Sensorgesteuert ist und würde den Rahmen für diese Arbeit sprengen.

## Datensicherheit

Das System läuft in einem Netzwerk, welches keinen Zugriff nach aussen hat und ist bissher nicht weiter geschützt. In einem zukünfitigen Schritt muss das System nach aussen freigeschaltet werden und die Website muss hinter einem geschützten User-System gesichert werden.

## Deployment

Der jeweilige Code konnte erfolgreich kompiliert und auf die Zielsysteme geladen werden.
Der PHP Code läuft auf einem Raspberry Pi 3B+ mit Apache V2 und einem MariaDB SQL Server.

Der Code vom ESP32 wurde per USB auf das System geladen. Das Development board wurde dann auf das im Rahmen der Diplomarbeit erstellten Leiterplatine gesteckt und getestet.

## Maintenance & Support

Fehler und sonstige Probleme mit dem System können per GITHUB als Bug report oder per E-MAIL an suter.eric@hotmail.com gesendet werden.
Sowohl die Hard- und Software befinden sich in einer Alpha Phase, und ein End-of-Life ist vorerst noch nicht absehbar. Im Fall, dass die Hard, oder SOftware End-Of-Life erreicht, wird dies per GITHUB bekannt gegeben.


