<?php
include_once "html_snippets/navbar.php";
create_navbar();

session_start();
// Check if session exists and isClient equals 0
if (!isset($_SESSION['email']) || $_SESSION['isClient'] != 0) {
    header('Location: /index.html'); // Redirect to index page
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Add Pizza</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Add New Pizza</h1>
        <form action="/scripts/insert_pizza.php" method="post">
            <div class="form-group">
                <label for="nome">Name:</label>
                <input type="text" class="form-control" id="nome" name="nome" required>
            </div>
            <div class="form-group">
                <label for="prezzo">Price:</label>
                <input type="number" step="0.01" class="form-control" id="prezzo" name="prezzo" required>
            </div>
            <div class="form-group">
                <label for="nomeTipo">Type:</label>
                <select class="form-control" id="nomeTipo" name="nomeTipo" required>
                    <?php
                    // Connect to the database and fetch the types
                    $data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

                    $conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);
                    $query = "SELECT * FROM tipi_pizza";
                    $result = $conn->query($query);

                    // Loop through the types and create the options
                    while ($row = $result->fetch_assoc()) {
                        echo "<option value='" . $row['nomeTipo'] . "'>" . $row['nomeTipo'] . "</option>";
                    }

                    // Close the database connection
                    $conn->close();
                    ?>
                </select>
            </div>
            <div class="form-group">
                <label for="ingredienti">Ingredients:</label>
                <?php
                // Connect to the database and fetch the ingredients
                $conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);
                $query = "SELECT * FROM ingredienti";
                $result = $conn->query($query);

                // Loop through the ingredients and create the checkboxes
                while ($row = $result->fetch_assoc()) {
                    echo "<div class='form-check'>";
                    echo "<input class='form-check-input' type='checkbox' name='ingredienti[]' value='" . $row['nome'] . "' id='" . $row['nome'] . "'>";
                    echo "<label class='form-check-label' for='" . $row['nome'] . "'>";
                    echo $row['nome'];
                    echo "</label>";
                    echo "</div>";
                }

                // Close the database connection
                $conn->close();
                ?>
            </div>
            <button type="submit" class="btn btn-primary">Add Pizza</button>
        </form>
    </div>
    <!-- Bootstrap JS -->
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
