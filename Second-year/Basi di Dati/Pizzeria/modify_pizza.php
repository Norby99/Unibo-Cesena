<?php
include_once "html_snippets/navbar.php";
create_navbar();

$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$pizzaName = $_GET['id'];

$stmt = $conn->prepare("SELECT p.nome, p.prezzo, p.nomeTipo, GROUP_CONCAT(i.nome SEPARATOR ', ') AS ingredienti, p.vendute
                        FROM pizze p
                        LEFT JOIN composizione c ON p.nome = c.nomePizza
                        LEFT JOIN ingredienti i ON c.nomeIngrediente = i.nome
                        WHERE p.nome = ?
                        GROUP BY p.nome");
$stmt->bind_param("s", $pizzaName);

$stmt->execute();

$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $pizza = $result->fetch_assoc();

    echo "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'>"; // Include Bootstrap CSS
    echo "<div class='container'>";
    echo "<form action='scripts/update_pizza.php' method='post' class='form-group'>";
    echo "<div class='form-group'>";
    echo "<label for='nomePizza'>Nome pizza:</label>";
    echo "<input type='text' id='nomePizza' name='nomePizza' value='" . $pizza['nome'] . "' class='form-control'>";
    echo "</div>";
    echo "<div class='form-group'>";
    echo "<label for='prezzo'>Prezzo:</label>";
    echo "<input type='text' id='prezzo' name='prezzo' value='" . $pizza['prezzo'] . "' class='form-control'>";
    echo "</div>";
?>
    <div class="form-group">
        <label for="ingredienti">Ingredietni:</label>
        <?php
        $query = "SELECT * FROM ingredienti";
        $result = $conn->query($query);

        $selectedIngredients = explode(", ", $pizza['ingredienti']);

        while ($row = $result->fetch_assoc()) {
            echo "<div class='form-check'>";
            echo "<input class='form-check-input' type='checkbox' name='ingredienti[]' value='" . $row['nome'] . "' id='" . $row['nome'] . "'";
            
            if (in_array($row['nome'], $selectedIngredients)) {
                echo " checked";
            }
            
            echo ">";
            echo "<label class='form-check-label' for='" . $row['nome'] . "'>";
            echo $row['nome'];
            echo "</label>";
            echo "</div>";
        }
        ?>
    </div>
<?php
    echo "<input type='submit' value='Aggiorna Pizza' class='btn btn-primary'>";
    echo "</form>";

    echo "<a href='scripts/remove_pizza.php?nome=" . $pizza['nome'] . "' class='btn btn-danger'>Rimuovi Pizza</a>";
} else {
    echo "<div class='alert alert-danger' role='alert'>Pizza non trovata</div>";
}

$stmt->close();
$conn->close();
?>
