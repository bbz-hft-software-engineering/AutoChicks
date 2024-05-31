<?php
$servername = "localhost";
$username = "chicken";
$password = "Rooster!";
$dbname = "henhouse";

// Verbindung erstellen
$conn = new mysqli($servername, $username, $password, $dbname);

// Verbindung überprüfen
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Daten aus dem POST-Request holen
$door_automatic = $_POST['door_automatic'];
$door_override = $_POST['door_override'];
$back_to_auto = $_POST['back_to_auto'];
$day = $_POST['day'];
$month = $_POST['month'];
$year = $_POST['year'];
$hours = $_POST['hours'];
$minutes = $_POST['minutes'];
$door = $_POST['door'];

// SQL-Abfrage zum Einfügen oder Aktualisieren der Einstellungen
$sql = "INSERT INTO settings (door_automatic, door_override, back_to_auto, day, month, year, hours, minutes, door)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
        ON DUPLICATE KEY UPDATE 
        door_automatic=VALUES(door_automatic), door_override=VALUES(door_override), back_to_auto=VALUES(back_to_auto), 
        day=VALUES(day), month=VALUES(month), year=VALUES(year), hours=VALUES(hours), minutes=VALUES(minutes), door=VALUES(door)";

$stmt = $conn->prepare($sql);
$stmt->bind_param("iiiiiiiii", $door_automatic, $door_override, $back_to_auto, $day, $month, $year, $hours, $minutes, $door);

if ($stmt->execute() === TRUE) {
    echo "Einstellungen erfolgreich gespeichert.";
} else {
    echo "Fehler: " . $stmt->error;
}

$stmt->close();
$conn->close();
?>