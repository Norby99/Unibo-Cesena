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

// delete every pizza from the orders of the user
$sql = "DELETE c
        FROM ordini o
        INNER JOIN comprende c ON o.idOrdine = c.idOrdine
        WHERE o.idUtente = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $_SESSION['id']);
$stmt->execute();

// delete every order from the ordini table
$sql = "DELETE FROM ordini
        WHERE idUtente = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $_SESSION['id']);
$stmt->execute();

// delete every prenotazioni_tavolo
$sql = "DELETE FROM prenotazioni_tavolo
        WHERE idUtente = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $_SESSION['id']);
$stmt->execute();

// delete the user from the utenti table
$sql = "DELETE FROM utenti
        WHERE idUtente = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $_SESSION['id']);
$stmt->execute();

$stmt->close();
$conn->close();

header("Location: /index.html");
?>
