<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
// MySQL-Verbindungsinformationen
$servername = "localhost"; // Normalerweise ist der Servername "localhost", wenn die Datenbank auf demselben Server wie die Webseite läuft
$username = "chicken";
$password = "Rooster!";
$dbname = "henhouse";

// POST-Daten erhalten
$Systemstatus = $_POST['Systemstatus'];
$Tuerstatus = $_POST['Tuerstatus'];
$Luefterstatus = $_POST['Luefterstatus'];
$Torstatus = $_POST['Torstatus'];
$Neststatus = $_POST['Neststatus'];
$Aussentemperatur = $_POST['Aussentemperatur'];
$Innentemperatur = $_POST['Innentemperatur'];
$Wassertemperatur = $_POST['Wassertemperatur'];
$Lichtwert = $_POST['Lichtwert'];
$Batteriespannung = $_POST['Batteriespannung'];
$Eintragszeit = $_POST['Eintragszeit'];
$Errortable = $_POST['Errortable'];
$Warningtable = $_POST['Warningtable'];

// Verbindung zur MySQL-Datenbank herstellen
$conn = new mysqli($servername, $username, $password, $dbname);

// Überprüfen, ob die Verbindung erfolgreich war
if ($conn->connect_error) {
    die("Verbindung zur MySQL-Datenbank fehlgeschlagen: " . $conn->connect_error);
}

// SQL-Abfrage zum Einfügen eines neuen Eintrags
$sql = "INSERT INTO status (systemstatus, tuerstatus, aussentemperatur, innentemperatur, luefterstatus, wassertemperatur, lichtwert, torstatus, neststatus, batteriespannung, eintragszeit, errortable, warningtable) 
        VALUES ('$Systemstatus', '$Tuerstatus', $Aussentemperatur, $Innentemperatur, $Luefterstatus, $Wassertemperatur, $Lichtwert, $Torstatus, $Neststatus, $Batteriespannung, $Eintragszeit, $Errortable, $Warningtable)";
// Weitere POST-Daten hier hinzufügen...

if ($conn->query($sql) === TRUE) {
    echo "Neuer Eintrag erfolgreich hinzugefügt.";
} else {
    echo "Fehler beim Hinzufügen des Eintrags: " . $conn->error;
}

// Verbindung schließen
$conn->close();
?>