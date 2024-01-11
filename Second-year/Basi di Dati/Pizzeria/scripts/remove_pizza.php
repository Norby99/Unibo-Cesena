<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$pizzaName = $_GET['nome'];

$stmt = $conn->prepare("SELECT *
                        FROM comprende
                        WHERE nomePizza = ?");

$stmt->bind_param("s", $pizzaName);
$stmt->execute();
$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $stmt->close();
    $conn->close();

    echo "<script>
            alert('Non puoi eliminare una pizza che è presente in un ordine.');
            window.location.href='/pizze.php';
          </script>"; 
} else {
    $stmt = $conn->prepare("DELETE FROM composizione WHERE nomePizza = ?");
    $stmt->bind_param("s", $pizzaName);
    $stmt->execute();

    echo "Deleted all ingredients from composizione for the given pizza";

    $stmt = $conn->prepare("DELETE FROM pizze WHERE nome = ?");
    $stmt->bind_param("s", $pizzaName);

    $stmt->execute();

    echo "Deleted the pizza";

    $stmt->close();
    $conn->close();

    header("Location: /pizze.php");
}
?>
