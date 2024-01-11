<?php
session_start();

$document_root = $_SERVER['DOCUMENT_ROOT'];
$setup = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $setup['dbUserName'];
$password = $setup['dbPassword'];
$dbname = $setup['dbName'];

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$idOrdine = $_GET['idOrdine'];

$sql = "UPDATE ordini SET processato = 1 WHERE idOrdine = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param('i', $idOrdine);

$stmt->execute();

header("Location: /orders_to_prepare.php");
?>
