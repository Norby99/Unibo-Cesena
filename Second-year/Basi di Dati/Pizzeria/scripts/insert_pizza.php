<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Prepare and bind
$stmt = $conn->prepare("INSERT INTO pizze (nome, prezzo, nomeTipo)
                        VALUES (?, ?, ?)");
$stmt->bind_param("sss", $_POST['nome'], $_POST['prezzo'], $_POST['nomeTipo']);
$stmt->execute();

echo "New pizza added successfully";

// Prepare and bind
$stmt = $conn->prepare("INSERT INTO composizione (nomePizza, nomeIngrediente)
                        VALUES (?, ?)");

foreach ($_POST['ingredienti'] as $ingrediente) {
    $stmt->bind_param("ss", $_POST['nome'], $ingrediente);
    $stmt->execute();
}

echo "Ingredients added successfully to the pizza";

// Close the database connection
$stmt->close();
$conn->close();

header('Location: /pizze.php');
?>
