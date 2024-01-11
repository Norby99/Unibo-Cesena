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

// Check if there are more than 4 orders the same day and time
$sql = "SELECT COUNT(*) AS count
        FROM ordini
        WHERE dataOrdine = ? AND oraInizio = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param('ss', $deliveryDay, $deliveryTime);
$stmt->execute();

$result = $stmt->get_result();
$count = $result->fetch_assoc()['count'];

if ($count >= 4) {
    echo "<script>
            alert('Too many orders at the same time!');
            window.location.href='/homepage.php';
          </script>";
}

// check if the user has already ordered at the same time
$sql = "SELECT COUNT(*) AS count
        FROM ordini
        WHERE idUtente = ? AND dataOrdine = ? AND oraInizio = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param('iss', $_SESSION['id'], $deliveryDay, $deliveryTime);
$stmt->execute();

$result = $stmt->get_result();
$count = $result->fetch_assoc()['count'];

if ($count > 0) {
    echo "<script>
            alert('You have already ordered at the same time!');
            window.location.href='/homepage.php';
          </script>";
}


// Creating the order
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

// Insert each pizza in comprende table
$orderId = $stmt->insert_id;
$basket = $_SESSION['basket'];

$pizzaCount = array(); // Associative array to store pizza count

foreach ($basket as $pizza) {
    $pizzaName = $pizza;
    
    if (isset($pizzaCount[$pizzaName])) {
        $pizzaCount[$pizzaName]++;
    } else {
        $pizzaCount[$pizzaName] = 1;
    }
}

foreach ($pizzaCount as $pizzaName => $count) {
    $sql = "INSERT INTO comprende (idOrdine, nomePizza, quantita)
            VALUES (?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param('isi', $orderId, $pizzaName, $count);
    $stmt->execute();
}

// Update the "pizza vendute" field in the database for each pizza in the basket
foreach ($pizzaCount as $pizzaName => $count) {
    $sql = "UPDATE pizze
            SET vendute = vendute + ?
            WHERE nome = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param('is', $count, $pizzaName);
    $stmt->execute();
}


// Clear the basket
$_SESSION['basket'] = array();

// Redirect to the confirmation page
header("Location: /homepage.php");
?>