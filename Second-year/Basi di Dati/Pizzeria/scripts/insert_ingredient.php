<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the ingredient data from the POST request
$nome = $_POST['nome'];
$descrizione = $_POST['descrizione'];
$nomeAllergene = $_POST['nomeAllergene'];

if ($nomeAllergene == "Nessuno") {
    $nomeAllergene = null;
}

// Prepare and bind
$stmt = $conn->prepare("INSERT INTO ingredienti (nome, descrizione, nomeAllergene) VALUES (?, ?, ?)");
$stmt->bind_param("sss", $nome, $descrizione, $nomeAllergene);

// Execute the statement
$stmt->execute();

// Close the database connection
$stmt->close();
$conn->close();

// Redirect back to the show ingredients page
header("Location: /ingredienti.php");
?>
