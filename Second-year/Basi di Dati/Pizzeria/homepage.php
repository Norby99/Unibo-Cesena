<?php
include_once "html_snippets/navbar.php";
create_navbar();

session_start();

// Check if session exists and isClient equals 0
if (!isset($_SESSION['email'])) {
    header('Location: /index.html'); // Redirect to index page
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Homepage</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Welcome, <?php echo $_SESSION['name']; ?>!</h1>
        <?php
        if($_SESSION['isClient'] == 0) {
        ?>
        <a href="/pizze.php" class="btn btn-primary">Pizze</a>
        <a href="/ingredienti.php" class="btn btn-primary">Ingredienti</a>
        <a href="/classifiche_pizze.php" class="btn btn-primary">Classifiche Pizze</a>
        <!-- mostra tutti gli ordini in assoluto -->
        <!-- mostra tutti gli ordini da preparare -->
        <?php } else { ?>
        <a href="/listino_pizze.php" class="btn btn-primary">Listino pizze</a>
        <?php } ?>
    </div>
    <!-- Bootstrap JS -->
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
