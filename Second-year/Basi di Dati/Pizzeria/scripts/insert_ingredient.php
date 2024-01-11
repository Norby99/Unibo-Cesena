<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$nome = $_POST['nome'];
$descrizione = $_POST['descrizione'];
$nomeAllergene = $_POST['nomeAllergene'];

if ($nomeAllergene == "Nessuno") {
    $nomeAllergene = null;
}

$stmt = $conn->prepare("INSERT INTO ingredienti (nome, descrizione, nomeAllergene) VALUES (?, ?, ?)");
$stmt->bind_param("sss", $nome, $descrizione, $nomeAllergene);

$stmt->execute();

$stmt->close();
$conn->close();

header("Location: /ingredienti.php");
?>
