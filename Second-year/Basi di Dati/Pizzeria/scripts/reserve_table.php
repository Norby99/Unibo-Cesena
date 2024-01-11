<?php
session_start();

$document_root = $_SERVER['DOCUMENT_ROOT'];
$data = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $data['dbUserName'];
$password = $data['dbPassword'];
$dbname = $data['dbName'];

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "INSERT INTO prenotazioni_tavolo (idTavolo, idUtente, dataPrenotazione, oraInizio)
        VALUES (?, ?, ?, ?)";
$stmt = $conn->prepare($sql);
$stmt->bind_param("ssss", $_POST['table'], $_SESSION['id'], $_POST['date'], $_POST['time']);
$stmt->execute();

echo 'Reservation completed successfully.'

$stmt->close();
$conn->close();

header("Location: /homepage.php");
?>
