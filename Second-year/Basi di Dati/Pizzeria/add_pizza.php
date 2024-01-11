<?php
include_once "html_snippets/navbar.php";
create_navbar();

session_start();
if (!isset($_SESSION['email']) || $_SESSION['isClient'] != 0) {
    header('Location: /index.html');
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Add Pizza</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Nuova pizza</h1>
        <form action="/scripts/insert_pizza.php" method="post">
            <div class="form-group">
                <label for="nome">Nome pizza:</label>
                <input type="text" class="form-control" id="nome" name="nome" required>
            </div>
            <div class="form-group">
                <label for="prezzo">Prezzo:</label>
                <input type="number" step="0.01" class="form-control" id="prezzo" name="prezzo" required>
            </div>
            <div class="form-group">
                <label for="nomeTipo">Tipo:</label>
                <select class="form-control" id="nomeTipo" name="nomeTipo" required>
                    <?php
                    $data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

                    $conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);
                    $query = "SELECT * FROM tipi_pizza";
                    $result = $conn->query($query);

                    while ($row = $result->fetch_assoc()) {
                        echo "<option value='" . $row['nomeTipo'] . "'>" . $row['nomeTipo'] . "</option>";
                    }

                    $conn->close();
                    ?>
                </select>
            </div>
            <div class="form-group">
                <label for="ingredienti">Ingredienti:</label>
                <?php
                $conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);
                $query = "SELECT * FROM ingredienti";
                $result = $conn->query($query);

                while ($row = $result->fetch_assoc()) {
                    echo "<div class='form-check'>";
                    echo "<input class='form-check-input' type='checkbox' name='ingredienti[]' value='" . $row['nome'] . "' id='" . $row['nome'] . "'>";
                    echo "<label class='form-check-label' for='" . $row['nome'] . "'>";
                    echo $row['nome'];
                    echo "</label>";
                    echo "</div>";
                }

                $conn->close();
                ?>
            </div>
            <button type="submit" class="btn btn-primary">Add Pizza</button>
        </form>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
