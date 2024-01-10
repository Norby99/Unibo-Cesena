<?php
session_start();

// Get the pizza index from the GET request
$pizzaIndex = $_GET['id'];

// Check if the basket session variable exists and is an array
if (isset($_SESSION['basket']) && is_array($_SESSION['basket'])) {
    // Check if the pizza index exists in the basket
    if (isset($_SESSION['basket'][$pizzaIndex])) {
        // Remove the pizza from the basket
        unset($_SESSION['basket'][$pizzaIndex]);

        // Re-index the basket array to ensure the indexes are sequential
        $_SESSION['basket'] = array_values($_SESSION['basket']);
    }
}

// Redirect back to the carrello page
header("Location: /carrello.php");
?>