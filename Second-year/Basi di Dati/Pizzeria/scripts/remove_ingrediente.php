<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the ingredient name from the POST request
$nome = $_GET['nome'];

// Prepare and bind
$stmt = $conn->prepare("DELETE FROM ingredienti WHERE nome = ?");
$stmt->bind_param("s", $nome);

// Execute the statement
$stmt->execute();

// Close the database connection
$stmt->close();
$conn->close();

// Redirect back to the show ingredients page
header("Location: /ingredienti.php");
?>
