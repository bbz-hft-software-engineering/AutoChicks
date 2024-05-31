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

// SQL-Abfrage für den neuesten Eintrag
$sql = "SELECT MAX(id) as max_id FROM status";
$result = $conn->query($sql);
$row = $result->fetch_assoc();

echo $row['max_id'];

$conn->close();
?>