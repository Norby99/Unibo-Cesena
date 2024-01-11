<?php
session_start();

$pizzaName = $_GET['id'];


if (!isset($_SESSION['basket'])) {
    $_SESSION['basket'] = array();
}

if (count($_SESSION['basket']) >= 10) {
    echo "<script>
            alert('Si può ordinare al massimo 10 pizze per ordine. Controllare il proprio carrello.');
            window.location.href='/listino_pizze.php';
          </script>";
} else {
    $_SESSION['basket'][] = $pizzaName;

    echo "Basket: ";
    print_r($_SESSION['basket']);
    
    header("Location: /listino_pizze.php");
}
?>
