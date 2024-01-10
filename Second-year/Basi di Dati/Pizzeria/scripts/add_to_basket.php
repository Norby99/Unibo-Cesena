<?php
session_start();

// Get the pizza name from the GET request
$pizzaName = $_GET['id'];


if (!isset($_SESSION['basket'])) {
    $_SESSION['basket'] = array();
}
# print every session variable
print_r($_SESSION);

// Check if the basket has reached the maximum limit
if (count($_SESSION['basket']) >= 10) {
    echo "<script>
            alert('Si può ordinare al massimo 10 pizze per ordine. Controllare il proprio carrello.');
            window.location.href='/listino_pizze.php';
          </script>";
} else {

    // Add the pizza to the basket
    $_SESSION['basket'][] = $pizzaName;

    // Print the basket
    echo "Basket: ";
    print_r($_SESSION['basket']);

    // Redirect back to the listino pizze page
    header("Location: /listino_pizze.php");
}
?>
