<?php
include_once "html_snippets/navbar.php";
create_navbar();

$document_root = $_SERVER['DOCUMENT_ROOT'];
$setup = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $setup['dbUserName'];
$password = $setup['dbPassword'];
$dbname = $setup['dbName'];

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT p.nome, p.prezzo, p.nomeTipo, GROUP_CONCAT(i.nome SEPARATOR ', ') AS ingredienti, p.vendute
        FROM pizze p
        INNER JOIN composizione c ON p.nome = c.nomePizza
        INNER JOIN ingredienti i ON c.nomeIngrediente = i.nome
        GROUP BY p.nome
        ORDER BY p.vendute DESC";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Classifica Pizze</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Classifica Pizze</h1>
        <ul class="list-group">
            <?php
            if ($result->num_rows > 0) {
                while($row = $result->fetch_assoc()) {
                    echo '<li class="list-group-item">';
                    echo $row['nome'] . ' - ' . $row['nomeTipo'];
                    echo '<br>Ingredienti: ' . $row['ingredienti'];
                    echo '<br>Prezzo: ' . $row['prezzo'];
                    echo '<br>Vendute: ' . $row['vendute'];
                    echo '</li>';
                }
            } else {
                echo "Non ci sono pizze";
            }
            ?>
        </ul>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>

<?php
$conn->close();
?>
