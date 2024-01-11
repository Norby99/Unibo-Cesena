<?php
session_start();

$pizzaIndex = $_GET['id'];

if (isset($_SESSION['basket']) && is_array($_SESSION['basket'])) {
    if (isset($_SESSION['basket'][$pizzaIndex])) {
        unset($_SESSION['basket'][$pizzaIndex]);

        $_SESSION['basket'] = array_values($_SESSION['basket']);
    }
}

header("Location: /carrello.php");
?>