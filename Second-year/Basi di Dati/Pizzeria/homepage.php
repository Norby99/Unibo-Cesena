<?php
// homepage.php
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
    <title>Homepage</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Welcome, <?php echo $_SESSION['name']; ?>!</h1>
        <a href="/pizze.php" class="btn btn-primary">Pizze</a>
        <a href="/ingredienti.php" class="btn btn-primary">Ingredienti</a>
    </div>
    <!-- Bootstrap JS -->
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
