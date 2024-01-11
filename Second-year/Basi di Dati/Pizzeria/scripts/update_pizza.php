<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$pizzaName = $_POST['nomePizza'];
$price = $_POST['prezzo'];
$ingredients = $_POST['ingredienti'];

$stmt = $conn->prepare("UPDATE pizze SET prezzo = ? WHERE nome = ?");
$stmt->bind_param("ds", $price, $pizzaName);

$stmt->execute();

$stmt = $conn->prepare("DELETE FROM composizione WHERE nomePizza = ?");
$stmt->bind_param("s", $pizzaName);
$stmt->execute();

foreach ($ingredients as $ingredient) {
    $stmt = $conn->prepare("INSERT INTO composizione (nomePizza, nomeIngrediente) VALUES (?, ?)");
    $stmt->bind_param("ss", $pizzaName, $ingredient);
    $stmt->execute();
}

$stmt->close();
$conn->close();

header("Location: /pizze.php");
?>
