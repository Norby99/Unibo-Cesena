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

// Get the order details from the POST request
$deliveryType = $_POST['delivery'];
$deliveryDay = $_POST['delivery-day'];
$deliveryDay = date('Y-m-d', strtotime($deliveryDay));
$deliveryTime = $_POST['delivery-time'];
$notes = $_POST['notes'];

if ($deliveryType == 'delivery') {
    $address = $_POST['address'];
    $addressParts = explode(' ', $address);
    $houseNumber = end($addressParts);
    $street = implode(' ', array_slice($addressParts, 0, -1));
    
    $sql = "INSERT INTO ordini (idUtente, oraInizio, dataOrdine, nCivicoDiConsegna, viaDiConsegna, note)
        VALUES (?, ?, ?, ?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param('isssss', $_SESSION['id'], $deliveryTime, $deliveryDay, $houseNumber, $street, $notes);
} else {
    $sql = "INSERT INTO ordini (idUtente, oraInizio, dataOrdine, note)
        VALUES (?, ?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param('isss', $_SESSION['id'], $deliveryTime, $deliveryDay, $notes);
}

// Execute the SQL statement
$stmt->execute();

// TODO add the pizzas to the order

// Clear the basket
//$_SESSION['basket'] = array();

// Redirect to the confirmation page
//header("Location: /homepage.php");
?>