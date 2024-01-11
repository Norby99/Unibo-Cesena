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

// Prepare the SQL statement
$sql = "SELECT idOrdine, dataOrdine, oraInizio, viaDiConsegna, nCivicoDiConsegna, note
        FROM ordini
        ORDER BY dataOrdine DESC, oraInizio DESC";

$stmt = $conn->prepare($sql);
// Execute the SQL statement
$stmt->execute();
// Get the result
$result = $stmt->get_result();

// Fetch all rows as an associative array
$orders = $result->fetch_all(MYSQLI_ASSOC);

// Loop through each order
foreach ($orders as $key => $order) {
    $pizzas = array();
    
    $sql = "SELECT nomePizza, quantita
            FROM comprende
            WHERE idOrdine = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param('i', $order['idOrdine']);
    $stmt->execute();

    $result = $stmt->get_result();
    $pizzas_ordered = $result->fetch_all(MYSQLI_ASSOC);

    // Loop through each pizza ordered
    foreach ($pizzas_ordered as $pizza_ordered) {
        // If the pizza is already in the array, add the quantity
        if (array_key_exists($pizza_ordered['nomePizza'], $pizzas)) {
            $pizzas[$pizza_ordered['nomePizza']] += $pizza_ordered['quantita'];
        } else {
            // Otherwise, add the pizza to the array
            $pizzas[$pizza_ordered['nomePizza']] = $pizza_ordered['quantita'];
        }
    }

    $pizzaStr = '';
    foreach ($pizzas as $pizza => $quantity) {
        $pizzaStr .= $pizza . ' x' . $quantity . '<br>';
    }
    $orders[$key]['pizze'] = $pizzaStr;
}

// Close the connection
$stmt->close();
$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Order History</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <?php include 'html_snippets/navbar.php'; create_navbar(); ?>
        <div class="container">
            <h1>Order History</h1>
            <table class="table">
                <thead>
                    <tr>
                        <th scope="col">#</th>
                        <th scope="col">Data ordine</th>
                        <th scope="col">Ora ordine</th>
                        <th scope="col">Indirizzo di consegna</th>
                        <th scope="col">Pizze</th>
                        <th scope="col">Note</th>
                    </tr>
                </thead>
                <tbody>
                    <?php
                    foreach ($orders as $key => $order) {
                        echo '<tr>';
                        echo '<th scope="row">' . ($key + 1) . '</th>';
                        echo '<td>' . $order['dataOrdine'] . '</td>';
                        echo '<td>' . $order['oraInizio'] . '</td>';
                        echo '<td>' . $order['viaDiConsegna'] . ' ' . $order['nCivicoDiConsegna'] . '</td>';
                        echo '<td>' . $order['pizze'] . '</td>';
                        echo '<td>' . $order['note'] . '</td>';
                        echo '</tr>';
                    }
                    ?>
                </tbody>
            </table>
        </div>
        <!-- Bootstrap JS -->
        <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    </body>
    </html>
