<?php
include_once "html_snippets/navbar.php";
create_navbar();

// Get DB details from setup.json
$document_root = $_SERVER['DOCUMENT_ROOT'];
$setup = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $setup['dbUserName'];
$password = $setup['dbPassword'];
$dbname = $setup['dbName'];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT p.nome, p.prezzo, p.nomeTipo, GROUP_CONCAT(i.nome SEPARATOR ', ') AS ingredienti, p.vendute
        FROM pizze p
        INNER JOIN composizione c ON p.nome = c.nomePizza
        INNER JOIN ingredienti i ON c.nomeIngrediente = i.nome
        GROUP BY p.nome";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pizze</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Pizze</h1>
        <ul class="list-group">
            <?php
            if ($result->num_rows > 0) {
                // Output data of each row
                while($row = $result->fetch_assoc()) {
                    echo '<li class="list-group-item">';
                    echo $row['nome'] . ' - ' . $row['nomeTipo'];
                    echo '<br>Ingredienti: ' . $row['ingredienti'];
                    echo '<br>Prezzo: ' . $row['prezzo'];
                    echo '<a href="/scripts/add_to_basket.php?id=' . $row['nome'] . '" class="btn btn-primary float-right">Aggiungi al carrello</a>';
                    echo '</li>';
                }
            } else {
                echo "No pizzas found";
            }
            ?>
        </ul>
        <a href="/carrello.php" class="btn btn-primary">Show Basket</a>
    </div>
    <!-- Bootstrap JS -->
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>

<?php
$conn->close();
?>
