<?php
include_once "html_snippets/navbar.php";
create_navbar();

session_start();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Basket</title>
    <!-- Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <div class="container">
        <h1>Basket</h1>
        <table class="table">
            <thead>
                <tr>
                    <th scope="col">#</th>
                    <th scope="col">Pizza</th>
                    <th scope="col">Action</th>
                </tr>
            </thead>
            <tbody>
                <?php
                if (isset($_SESSION['basket']) && is_array($_SESSION['basket'])) {
                    foreach ($_SESSION['basket'] as $key => $pizza) {
                        echo '<tr>';
                        echo '<th scope="row">' . ($key + 1) . '</th>';
                        echo '<td>' . $pizza . '</td>';
                        echo '<td><a href="scripts/remove_from_basket.php?id=' . urlencode($key) . '" class="btn btn-danger">Rimuovi</a></td>';
                        echo '</tr>';
                    }
                    echo "<a href='checkout.php' class='btn btn-primary'>Procedi all'ordine</a>";
                }
                ?>
            </tbody>
        </table>
    </div>
    <!-- Bootstrap JS -->
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>