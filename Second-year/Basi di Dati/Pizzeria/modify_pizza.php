<?php
$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);
// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Get the pizza id from the header
$pizzaName = $_GET['id'];

// Prepare and bind
$stmt = $conn->prepare("SELECT p.nome, p.prezzo, p.nomeTipo, GROUP_CONCAT(i.nome SEPARATOR ', ') AS ingredienti, p.vendute
                        FROM pizze p
                        INNER JOIN composizione c ON p.nome = c.nomePizza
                        INNER JOIN ingredienti i ON c.nomeIngrediente = i.nome
                        WHERE p.nome = ?
                        GROUP BY p.nome");
$stmt->bind_param("s", $pizzaName);

// Execute the statement
$stmt->execute();

// Get the result
$result = $stmt->get_result();

// Check if the pizza exists
if ($result->num_rows > 0) {
    // Fetch the pizza data
    $pizza = $result->fetch_assoc();

    // Display the form for modifying the pizza
    echo "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'>"; // Include Bootstrap CSS
    echo "<div class='container'>";
    echo "<form action='scripts/update_pizza.php' method='post' class='form-group'>";
    echo "<div class='form-group'>";
    echo "<label for='nomePizza'>Nome:</label>";
    echo "<input type='text' id='nomePizza' name='nomePizza' value='" . $pizza['nome'] . "' class='form-control'>";
    echo "</div>";
    echo "<div class='form-group'>";
    echo "<label for='prezzo'>Price:</label>";
    echo "<input type='text' id='prezzo' name='prezzo' value='" . $pizza['prezzo'] . "' class='form-control'>";
    echo "</div>";
?>
    <div class="form-group">
        <label for="ingredienti">Ingredients:</label>
        <?php
        $query = "SELECT * FROM ingredienti";
        $result = $conn->query($query);

        // Get the selected ingredients for the pizza
        $selectedIngredients = explode(", ", $pizza['ingredienti']);

        // Loop through the ingredients and create the checkboxes
        while ($row = $result->fetch_assoc()) {
            echo "<div class='form-check'>";
            echo "<input class='form-check-input' type='checkbox' name='ingredienti[]' value='" . $row['nome'] . "' id='" . $row['nome'] . "'";
            
            // Check the checkbox if it is in the selected ingredients
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
    echo "<input type='submit' value='Update Pizza' class='btn btn-primary'>";
    echo "</form>";
    echo "</div>";
} else {
    echo "<div class='alert alert-danger' role='alert'>Pizza not found</div>";
}

// Close the database connection
$stmt->close();
$conn->close();
?>
