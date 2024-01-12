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
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <link href="style/footer.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Bentornata/o, <?php echo $_SESSION['name']; ?>!</h1>
        <?php
        if($_SESSION['isClient'] == 0) {
        ?>
        <a href="/pizze.php" class="btn btn-primary">Pizze</a>
        <a href="/ingredienti.php" class="btn btn-primary">Ingredienti</a>
        <a href="/classifiche_pizze.php" class="btn btn-primary">Classifiche Pizze</a>
        <a href="/all_orders.php" class="btn btn-primary">Tutti gli ordini</a>
        <a href="/orders_to_prepare.php" class="btn btn-primary">Ordini da preparare</a>
        <a href="/reserved_tables_date.php" class="btn btn-primary">Tavoli prenotati</a>
        <?php } else { ?>
        <a href="/listino_pizze.php" class="btn btn-primary">Listino pizze</a>
        <a href="/orders_history.php" class="btn btn-primary">Cronologia ordini</a>
        <a href="/reserve_table_date.php" class="btn btn-primary">Prenota un tavolo</a>
        <?php } ?>
    </div>

    <?php if($_SESSION['isClient'] == 1) { ?>
    <footer class="d-flex flex-wrap justify-content-between align-items-center py-3 my-4 border-top">
        <form action="/scripts/delete_user.php" method="post">
            <button type="submit" class="btn btn-dark">Elimina Utente</button>
        </form>
    </footer>
    <?php } ?>
    
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
