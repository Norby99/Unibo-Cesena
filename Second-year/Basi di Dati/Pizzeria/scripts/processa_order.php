<?php
session_start();

$document_root = $_SERVER['DOCUMENT_ROOT'];
$setup = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $setup['dbUserName'];
$password = $setup['dbPassword'];
$dbname = $setup['dbName'];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the order ID from the GET request
$idOrdine = $_GET['idOrdine'];

// Prepare the SQL statement
$sql = "UPDATE ordini SET processato = 1 WHERE idOrdine = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param('i', $idOrdine);

// Execute the SQL statement
$stmt->execute();

// Redirect back to the orders to prepare page
header("Location: /orders_to_prepare.php");
?>
