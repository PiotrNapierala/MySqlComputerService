<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <style>
        .button {
            background-color: gray;
            color: white;
            padding: 10px;
            border: none;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 4px;
        }

        .center-container {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .upload-form {
            text-align: center;
        }

        #image-preview {
            border: 2px solid gray;
            border-radius: 6px;
            padding: 10px;
            margin: 20px;
            max-width: 100%;
            max-height: 100%;
            box-sizing: border-box;
        }

        #image-preview img {
            width: 100%;
            height: auto;
        }
    </style>
</head>

<body>
    <nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
        <a class="navbar-brand" href="#">Upload photo</a>
    </nav>
    <div class="center-container">
        <div class="upload-form">
            <?php
            $IP = $_SERVER["REMOTE_ADDR"];

            $servername = "#database_host#";
		    $username = "#datavase_user#";
		    $password = "#database_password#";
		    $dbname = "#database_database_name#";

            $conn = new mysqli($servername, $username, $password, $dbname);
            $conn->set_charset("utf8");

            if ($conn->connect_error) {
                die(
                    "Nie udało się połączyć z bazą danych: " .
                        $conn->connect_error
                );
            }

            $sql = "SELECT value FROM global_settings WHERE name = 'pin'";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
                $row = $result->fetch_assoc();
                $database_pin = $row["value"];

                if (isset($_POST["pin"])) {
                    if ($_POST["pin"] == $database_pin) {
                        $sql = "SELECT * FROM devices WHERE IP='$IP';";
                        $result = $conn->query($sql);

                        if ($result->num_rows > 0) {
                            if (isset($_GET["id"])) {
                                $id = $_GET["id"];
                                echo "<form action='upload.php?id=$id' method='post' enctype='multipart/form-data'>";
                                echo "<label for='fileToUpload' class='button'>Wybierz zdjęcie</label>";
                                echo "<input type='file' name='fileToUpload' id='fileToUpload' style='display: none;' accept='image/*' onchange='previewImage(event)'>";
                                echo "<div id='image-preview'></div>";
                                echo "<br><br>";
                                echo "<input type='submit' value='Wyślij' class='button'>";
                                echo "</form>";
                            } else {
                                header("Location: error.html");
                                exit();
                            }
                        } else {
                            echo "Błąd autoryzacji!";
                            echo $IP;
                        }

                        $conn->close();
                    } else {
                        echo "<div class='text-center'><form method='POST'><label for='pin'>Błędny kod pin<br>Podaj prawidłowy pin:</label><br><input type='password' name='pin' pattern='[0-9]*' inputmode='numeric' id='pin' required><br><button type='submit' class='btn btn-primary'>Zatwierdź</button></form></div>";
                    }
                } else {
                    echo "<div class='text-center'><form method='POST'><label for='pin'>Podaj pin dostępu:</label><br><input type='password' name='pin' pattern='[0-9]*' inputmode='numeric' id='pin' required><br><button type='submit' class='btn btn-primary'>Zatwierdź</button></form></div>";
                }
            } else {
                echo "Błąd autoryzacji!";
            }
            ?>
            <script>
                function previewImage(event) {
                var reader = new FileReader();
                reader.onload = function() {
                    var output = document.getElementById('image-preview');
                    output.innerHTML = '<img src="' + reader.result + '" alt="Podgląd zdjęcia">';
                };
                reader.readAsDataURL(event.target.files[0]);
            }
            </script>
        </div>
    </div>
</body>

</html>