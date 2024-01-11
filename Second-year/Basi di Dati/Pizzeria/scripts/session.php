<?php
session_start();

if (isset($_POST['email'])) {
    $_SESSION['email'] = $_POST['email'];
    $_SESSION['basket'] = array();
    echo "Session started for user: " . $_SESSION['email'];
} else {
    echo "No user logged in.";
}
?>
