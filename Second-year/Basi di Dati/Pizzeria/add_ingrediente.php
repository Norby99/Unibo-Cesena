<?php
include_once "html_snippets/navbar.php";
create_navbar();

// Include Bootstrap CSS
echo "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'>";

// Display the form for adding a new ingredient
echo "<div class='container'>";
echo "<form action='scripts/insert_ingredient.php' method='post' class='form-group'>";
echo "<div class='form-group'>";
echo "<label for='nome'>Name:</label>";
echo "<input type='text' id='nome' name='nome' class='form-control'>";
echo "</div>";
echo "<div class='form-group'>";
echo "<label for='descrizione'>Description:</label>";
echo "<input type='text' id='descrizione' name='descrizione' class='form-control'>";
echo "</div>";
echo "<div class='form-group'>";
echo "<label for='nomeAllergene'>Allergen Name:</label>";
echo "<select id='nomeAllergene' name='nomeAllergene' class='form-control'>";

$data = json_decode(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/setup.json'), true);

// Connect to the database
$conn = new mysqli("localhost", $data['dbUserName'], $data['dbPassword'], $data['dbName']);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Fetch all the ingredients
$query = "SELECT * FROM allergeni";
$result = $conn->query($query);

echo "<option value='Nessuno'>Nessuno</option>";
while ($row = $result->fetch_assoc()) {
    $allergen = $row['nome'];
    echo "<option value='$allergen'>$allergen</option>";
}

echo "</select>";
echo "</div>";
echo "<input type='submit' value='Add Ingredient' class='btn btn-primary'>";
echo "</form>";
echo "</div>";
?>
