<?php

$servername = "#database_host#";
$username = "#datavase_user#";
$password = "#database_password#";
$dbname = "#database_database_name#";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Nie udało się połączyć z bazą danych: " . $conn->connect_error);
} else {

if (isset($_GET['id'])) {
    $id = $_GET['id'];

    $target_dir = "uploads/";
    $originalFileName = $_FILES['fileToUpload']['name'];
    $fileParts = pathinfo($originalFileName);
    $fileName = $id . '.' . $fileParts['extension'];
    $target_file = $target_dir . $fileName;

    $uploadOk = 1;
    $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

    // Check if image file is a actual image or fake image
    if(isset($_POST["submit"])) {
      $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
      if($check !== false) {
        $uploadOk = 1;
      } else {
        $uploadOk = 0;
      }
    }

    // Check if file already exists
    if (file_exists($target_file)) {
      $uploadOk = 0;
    }

    // Check file size
    if ($_FILES["fileToUpload"]["size"] > 10000000) {
      $uploadOk = 0;
    }

    // Allow certain file formats
    if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
      && $imageFileType != "gif" ) {
      $uploadOk = 0;
    }

    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
        header('Location: error.html');
        exit;
    // if everything is ok, try to upload file
    } else {
      if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {

        $sql = "INSERT INTO images (name, value) VALUES ('$id', '$fileName') 
         ON DUPLICATE KEY UPDATE value = '$fileName'";

        if ($conn->query($sql) === TRUE) {
          header('Location: success.html');
          exit;
        } else {
          
          echo $conn->error();
        }

      } else {
        header('Location: error.html');
        exit;
      }
    }
} else {
    header('Location: error.html');
    exit;
}
}

$conn->close();

?>