<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>System Status</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
        h1 {
            color: #333;
        }
        .status, .settings {
            margin: 20px 0;
        }
        label {
            display: block;
            margin-top: 10px;
        }
        input, select {
            padding: 5px;
            margin-top: 5px;
        }
        button {
            margin-top: 20px;
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
        }
        button:hover {
            background-color: #45a049;
        }
    </style>
    <script>
        function submitSettings(event) {
            event.preventDefault(); // Verhindert das Standardverhalten des Formulars
            
            // Formular-Daten sammeln
            const formData = new FormData(document.getElementById('settingsForm'));

            // AJAX-Request mit Fetch-API senden
            fetch('submit_settings.php', {
                method: 'POST',
                body: formData
            })
            .then(response => response.text())
            .then(data => {
                alert(data); // Popup mit Rückmeldung anzeigen
            })
            .catch(error => {
                console.error('Error:', error);
                alert('Es gab ein Problem beim Speichern der Einstellungen.');
            });
        }

        let latestId = null;

        function checkForUpdates() {
            fetch('check_new_data.php')
            .then(response => response.text())
            .then(data => {
                if (latestId === null) {
                    latestId = data;
                } else if (data > latestId) {
                    latestId = data;
                    location.reload(); // Seite neu laden
                }
            })
            .catch(error => {
                console.error('Error:', error);
            });
        }

        // Prüfen Sie alle 10 Sekunden auf Updates
        setInterval(checkForUpdates, 10000);
    </script>
</head>
<body>
    <h1>System Status</h1>
    
    <div class="status">
        <?php
        // Verbindung zur MySQL-Datenbank herstellen
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
        $sql = "SELECT * FROM status ORDER BY id DESC LIMIT 1";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {

            define('ERROR_BAT', 0b000001);                
            define('ERROR_DOOR', 0b000010);    
            define('ERROR_DATE', 0b000100);    
            define('ERROR_SEND_DATA', 0b001000);
            define('WARNING_BAT', 0b000001);
            define('WARNING_GET_SETTINGS', 0b000010);
            define('WARNING_SEND_DATA', 0b000100);
            define('WARNING_WATER_TEMP', 0b001000);
            define('WARNING_AIR_TEMP', 0b010000);
            define('WARNING_NEST', 0b100000);
            define('WARNING_EXIT', 0b1000000);

            // Ergebnisse ausgeben
            while($row = $result->fetch_assoc()) {

                $systemstatusText = '';
                switch ($row["systemstatus"]) {
                    case 0:
                        $systemstatusText = 'OK';
                        break;
                    case 1:
                        $systemstatusText = 'WARNING';
                        break;
                    case 2:
                        $systemstatusText = 'ERROR';
                        break;
                }
                echo "<p>Systemstatus: " . htmlspecialchars($systemstatusText) . "</p>";

                $torstatusText = '';
                switch ($row["torstatus"]) {
                    case 0:
                        $torstatusText = 'Geschlossen';
                        break;
                    case 1:
                        $torstatusText = 'Offen';
                        break;
                }
                echo "<p>Torstatus: " . htmlspecialchars($torstatusText) . "</p>";

                $luefterstatusText = '';
                switch ($row["luefterstatus"]) {
                    case 0:
                        $luefterstatusText = 'Aus';
                        break;
                    case 1:
                        $luefterstatusText = 'Ein';
                        break;
                }                
                echo "<p>Lüfterstatus: " . htmlspecialchars($luefterstatusText) . "</p>";

                $neststatusText = '';
                switch ($row["neststatus"]) {
                    case 0:
                        $neststatusText = 'Leer';
                        break;
                    case 1:
                        $neststatusText = 'Besetzt';
                        break;
                }
                echo "<p>Neststatus: " . htmlspecialchars($neststatusText) . "</p>";

                echo "<p>Aussentemperatur: " . htmlspecialchars($row["aussentemperatur"]) . " °C" . "</p>";
                echo "<p>Innentemperatur: " . htmlspecialchars($row["innentemperatur"]) . " °C" . "</p>";
                echo "<p>Wassertemperatur: " . htmlspecialchars($row["wassertemperatur"]) . " °C" . "</p>";
                echo "<p>Lichtwert: " . htmlspecialchars($row["lichtwert"]) . "</p>";
                echo "<p>Batteriespannung: " . htmlspecialchars($row["batteriespannung"]) . " V" . "</p>";
                echo "<p>Eintragszeit: " . htmlspecialchars($row["eintragszeit"]) . " Uhr" . "</p>";
                
                echo "<p>Fehlerschlüssel: " . htmlspecialchars($row["errortable"]) . "</p>";
                if($row["errortable"] & ERROR_BAT){echo "<p>Akku leer.</p>";}
                if($row["errortable"] & ERROR_DOOR){echo "<p>Türfunktion gestört.</p>";}  
                if($row["errortable"] & ERROR_DATE){echo "<p>Zeiteinstellung verloren.</p>";}
                if($row["errortable"] & ERROR_SEND_DATA){echo "<p>Serververbindung verloren.</p>";}
                
                echo "<p>Warnungsschlüssel: " . htmlspecialchars($row["warningtable"]) . "</p>";
                if($row["warningtable"] & WARNING_BAT){echo "<p>Akkustand niedrig</p>";}             
                if($row["warningtable"] & WARNING_GET_SETTINGS){echo "<p>Einstellungen können nicht abgerufen werden.</p>";}
                if($row["warningtable"] & WARNING_SEND_DATA){echo "<p>Status konnte einmalig nicht übermittelt werden.</p>";}                   
                if($row["warningtable"] & WARNING_WATER_TEMP){echo "<p>Trinkwasser ist gefroren.</p>";}
                if($row["warningtable"] & WARNING_AIR_TEMP){echo "<p>Lufttemperatur ausserhalb der Norm.</p>";}                 
                if($row["warningtable"] & WARNING_NEST){echo "<p>Nest ist belegt nach Schliessungszeit.</p>";} 
                if($row["warningtable"] & WARNING_EXIT){echo "<p>Notausgang ist geöffnet</p>";} 
            }
        } else {
            echo "<p>Keine Daten gefunden.</p>";
        }
        $conn->close();
        ?>
    </div>

    <h1>Hühnerstall Einstellungen</h1>
    <div class="settings">
        <?php
        // Verbindung zur Datenbank erneut herstellen
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Verbindung überprüfen
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // Neuesten Eintrag abrufen
        $sql = "SELECT * FROM settings ORDER BY id DESC LIMIT 1";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
        } else {
            // Standardwerte, falls keine Einträge vorhanden sind
            $row = [
                'door_automatic' => 0,
                'door_override' => 0,
                'back_to_auto' => 0,
                'day' => 1,
                'month' => 1,
                'year' => 2024,
                'hours' => 0,
                'minutes' => 0
            ];
        }

        $conn->close();
        ?>

        <form id="settingsForm" onsubmit="submitSettings(event)">
            <label for="door_automatic">Türautomatik:</label>
            <select id="door_automatic" name="door_automatic">
                <option value="1" <?php echo $row['door_automatic'] == 1 ? 'selected' : ''; ?>>Ein</option>
                <option value="0" <?php echo $row['door_automatic'] == 0 ? 'selected' : ''; ?>>Aus</option>
            </select>

            <label for="door_override">Türoverride:</label>
            <select id="door_override" name="door_override">
                <option value="1" <?php echo $row['door_override'] == 1 ? 'selected' : ''; ?>>Ein</option>
                <option value="0" <?php echo $row['door_override'] == 0 ? 'selected' : ''; ?>>Aus</option>
            </select>
            <select id="door" name="door">
                <option value="1" <?php echo $row['door'] == 1 ? 'selected' : ''; ?>>Öffnen</option>
                <option value="0" <?php echo $row['door'] == 0 ? 'selected' : ''; ?>>Schliessen</option>
            </select>
            
            <label for="back_to_auto">Back to Auto:</label>
            <select id="back_to_auto" name="back_to_auto">
                <option value="1" <?php echo $row['back_to_auto'] == 1 ? 'selected' : ''; ?>>Ein</option>
                <option value="0" <?php echo $row['back_to_auto'] == 0 ? 'selected' : ''; ?>>Aus</option>
            </select>

            <label for="day">Datum [dd:mm:yyyy]: </label>
            <input type="number" id="day" name="day" min="1" max="31" value="<?php echo htmlspecialchars($row['day']); ?>" required>
            <input type="number" id="month" name="month" min="1" max="12" value="<?php echo htmlspecialchars($row['month']); ?>" required>
            <input type="number" id="year" name="year" min="2000" value="<?php echo htmlspecialchars($row['year']); ?>" required>

            <label for="hours">Uhrzeit [hh:mm]: </label>
            <input type="number" id="hours" name="hours" min="0" max="23" value="<?php echo htmlspecialchars($row['hours']); ?>" required>
            <input type="number" id="minutes" name="minutes" min="0" max="59" value="<?php echo htmlspecialchars($row['minutes']); ?>" required>
            <br>
            <button type="submit">Einstellungen speichern</button>
        </form>
    </div>
</body>
</html>
