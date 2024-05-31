<?php
$servername = "localhost";
$username = "chicken";
$password = "Rooster!";
$dbname = "henhouse";

// Erstelle eine Verbindung zur Datenbank
$conn = new mysqli($servername, $username, $password, $dbname);

// Überprüfe die Verbindung
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// SQL-Abfrage, um den neuesten Eintrag abzurufen
$sql = "SELECT * FROM settings ORDER BY id DESC LIMIT 1";
$result = $conn->query($sql);

$response = array();

if ($result->num_rows > 0) {
    // Ausgabe der Daten des neuesten Eintrags
    $response = $result->fetch_assoc();
} else {
    $response["error"] = "No entries found";
}

$conn->close();

// Rückgabe der Daten im JSON-Format
header('Content-Type: application/json');
echo json_encode($response);
?>