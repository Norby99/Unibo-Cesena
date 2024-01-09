<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the pizza data from the POST request
$pizzaName = $_POST['nomePizza'];
$price = $_POST['prezzo'];
$ingredients = $_POST['ingredienti'];

// Prepare and bind
$stmt = $conn->prepare("UPDATE pizze SET prezzo = ? WHERE nome = ?");
$stmt->bind_param("ds", $price, $pizzaName);

// Execute the statement
$stmt->execute();

// Update the ingredients
$stmt = $conn->prepare("DELETE FROM composizione WHERE nomePizza = ?");
$stmt->bind_param("s", $pizzaName);
$stmt->execute();

foreach ($ingredients as $ingredient) {
    $stmt = $conn->prepare("INSERT INTO composizione (nomePizza, nomeIngrediente) VALUES (?, ?)");
    $stmt->bind_param("ss", $pizzaName, $ingredient);
    $stmt->execute();
}

// Close the database connection
$stmt->close();
$conn->close();

// Redirect back to the modify pizza page
header("Location: /pizze.php");
?>
