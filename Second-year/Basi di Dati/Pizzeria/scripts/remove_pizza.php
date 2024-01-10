<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the pizza name from the GET request
$pizzaName = $_GET['nome'];

// Delete all ingredients from composizione for the given pizza
$stmt = $conn->prepare("DELETE FROM composizione WHERE nomePizza = ?");
$stmt->bind_param("s", $pizzaName);
$stmt->execute();

echo "Deleted all ingredients from composizione for the given pizza";

// Prepare and bind
$stmt = $conn->prepare("DELETE FROM pizze WHERE nome = ?");
$stmt->bind_param("s", $pizzaName);

// Execute the statement
$stmt->execute();

echo "Deleted the pizza";

// Close the database connection
$stmt->close();
$conn->close();

// Redirect back to the show pizzas page (replace with your actual page)
header("Location: /pizze.php");
?>
