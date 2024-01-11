<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$stmt = $conn->prepare("INSERT INTO pizze (nome, prezzo, nomeTipo)
                        VALUES (?, ?, ?)");
$stmt->bind_param("sss", $_POST['nome'], $_POST['prezzo'], $_POST['nomeTipo']);
$stmt->execute();

echo "New pizza added successfully";

$stmt = $conn->prepare("INSERT INTO composizione (nomePizza, nomeIngrediente)
                        VALUES (?, ?)");

foreach ($_POST['ingredienti'] as $ingrediente) {
    $stmt->bind_param("ss", $_POST['nome'], $ingrediente);
    $stmt->execute();
}

echo "Ingredients added successfully to the pizza";

$stmt->close();
$conn->close();

header('Location: /pizze.php');
?>
