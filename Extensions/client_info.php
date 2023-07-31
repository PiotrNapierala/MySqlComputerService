<!DOCTYPE html>
<html>

<head>
    <title>Dane klienta</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        body {
            padding-top: 60px;
        }

        table {
            border-collapse: collapse;
            width: 100%;
            max-width: 600px;
            margin: auto;
        }

        th,
        td {
            text-align: left;
            padding: 8px;
            border-bottom: 1px solid #ddd;
        }

        tr:hover {
            background-color: #f5f5f5;
        }
    </style>
</head>

<body>
    <nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
        <a class="navbar-brand" href="#">Client info</a>
    </nav>

    <main role="main" class="container">
        <?php

		$IP = $_SERVER['REMOTE_ADDR'];

		$servername = "#database_host#";
		$username = "#datavase_user#";
		$password = "#database_password#";
		$dbname = "#database_database_name#";

		$conn = new mysqli($servername, $username, $password, $dbname);
		$conn->set_charset('utf8');
				
		if ($conn->connect_error) {
			die("Nie udało się połączyć z bazą danych: " . $conn->connect_error);
		}

		$sql = "SELECT value FROM global_settings WHERE name = 'pin'";
		$result = $conn->query($sql);

		if ($result->num_rows > 0) {
			$row = $result->fetch_assoc();
			$database_pin = $row['value'];

			if(isset($_POST['pin'])) {
			
				if($_POST['pin'] == $database_pin) {
	
					$sql = "SELECT * FROM devices WHERE IP='$IP';";
					$result = $conn->query($sql);
	
					if ($result->num_rows > 0) {
	
						$id = $_GET['id'];
						$sql = "SELECT * FROM clients WHERE id='$id';";
						$result = $conn->query($sql);
	
						if ($result->num_rows > 0) {
							echo "<div class='table-responsive'><table class='table table-striped'>";
							while($row = $result->fetch_assoc()) {
								echo "<tr><th>Imię:</th><td>" . $row["name"]. "</td></tr><tr><th>Nazwisko:</th><td>" . $row["surname"]. "</td></tr><tr><th>NIP:</th><td>" . $row["nip"]. "</td></tr><tr><th>Email:</th><td><a href='mailto:" . $row["email"] . "'>" . $row["email"]. "</a></td></tr><tr><th>Telefon:</th><td><a href='tel:" . $row["phone"] . "'>" . $row["phone"] . "</a></td></tr>";
							}
							echo "</table></div>";
						} else {
							echo "Nie znaleziono klienta o podanym ID";
						}
	
					} else {
						echo "Błąd autoryzacji!";
						echo $IP;
					}
	
					$conn->close();
				}
				else {
					echo "<div class='text-center'><form method='POST'><label for='pin'>Błędny kod pin<br>Podaj prawidłowy pin:</label><br><input type='password' name='pin' pattern='[0-9]*' inputmode='numeric' id='pin' required><br><button type='submit' class='btn btn-primary'>Zatwierdź</button></form></div>";
				}
			} else {
				echo "<div class='text-center'><form method='POST'><label for='pin'>Podaj pin dostępu:</label><br><input type='password' name='pin' pattern='[0-9]*' inputmode='numeric' id='pin' required><br><button type='submit' class='btn btn-primary'>Zatwierdź</button></form></div>";
			}
			
		} else {
			echo "Błąd autoryzacji!";
		}

		?>
    </main>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.3/dist/umd/popperjs-core.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.0/dist/js/bootstrap.min.js"></script>
</body>

</html>