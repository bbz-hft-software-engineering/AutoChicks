<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
// MySQL-Verbindungsinformationen
$servername = "localhost"; // Normalerweise ist der Servername "localhost", wenn die Datenbank auf demselben Server wie die Webseite läuft
$username = "chicken";
$password = "Rooster!";
$dbname = "henhouse";

// POST-Daten erhalten
$door_automatic = $_POST['door_automatic'];
$door_override = $_POST['door_override'];
$back_to_auto = $_POST['back_to_auto'];
$day = $_POST['day'];
$month = $_POST['month'];
$year = $_POST['year'];
$hours = $_POST['hours'];
$minutes = $_POST['minutes'];
$door = $_POST['door'];

// Verbindung zur MySQL-Datenbank herstellen
$conn = new mysqli($servername, $username, $password, $dbname);

// Überprüfen, ob die Verbindung erfolgreich war
if ($conn->connect_error) {
    die("Verbindung zur MySQL-Datenbank fehlgeschlagen: " . $conn->connect_error);
}

// SQL-Abfrage zum Einfügen eines neuen Eintrags
$sql = "INSERT INTO settings (door_automatic, door_override, back_to_auto, day, month, year, hours, minutes, door) 
        VALUES ($door_automatic, $door_override, $back_to_auto, $day, $month, $year, $hours, $minutes, $door)";
// Weitere POST-Daten hier hinzufügen...

if ($conn->query($sql) === TRUE) {
    echo "Neuer Eintrag erfolgreich hinzugefügt.";
} else {
    echo "Fehler beim Hinzufügen des Eintrags: " . $conn->error;
}

// Verbindung schließen
$conn->close();
?>