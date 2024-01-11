<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$nome = $_GET['nome'];

# check if the ingredient is used in a recipe
$sql = "SELECT *
        FROM composizione
        WHERE nomeIngrediente = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $nome);

$stmt->execute();

$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $stmt->close();
    $conn->close();

    echo "<script>
            alert('Impossibile eliminare l\'ingrediente, è usato in una ricetta!');
            window.location.href='/ingredienti.php';
          </script>";
} else {
    $stmt = $conn->prepare("DELETE FROM ingredienti WHERE nome = ?");
    $stmt->bind_param("s", $nome);

    $stmt->execute();

    $stmt->close();
    $conn->close();

    header("Location: /ingredienti.php");
}
?>
