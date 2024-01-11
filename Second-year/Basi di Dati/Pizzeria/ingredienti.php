<?php
include_once "html_snippets/navbar.php";
create_navbar();

$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$query = "SELECT * FROM ingredienti";
$result = $conn->query($query);

echo "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'>";

echo "<div class='container'>";
echo "<table class='table'>";
echo "<thead class='thead-dark'>";
echo "<tr>";
echo "<th scope='col'>Nome</th>";
echo "<th scope='col'>Descrizione</th>";
echo "<th scope='col'>Nome Allergene</th>";
echo "<th scope='col'>Elimina</th>";
echo "</tr>";
echo "</thead>";
echo "<tbody>";
while ($row = $result->fetch_assoc()) {
    echo "<tr>";
    echo "<td>" . $row['nome'] . "</td>";
    echo "<td>" . $row['descrizione'] . "</td>";
    echo "<td>" . $row['nomeAllergene'] . "</td>";
    echo "<td>";
    echo "<a href='scripts/remove_ingrediente.php?nome=" . $row['nome'] . "' class='btn btn-danger'>Elimina</a>";
    echo "</td>";
    echo "</tr>";
    echo "</td>";
    echo "</tr>";
}
echo "</tbody>";
echo "</table>";

echo "<a href='add_ingrediente.php' class='btn btn-primary'>Nuovo Ingrediente</a>";

echo "</div>";

$conn->close();
?>
