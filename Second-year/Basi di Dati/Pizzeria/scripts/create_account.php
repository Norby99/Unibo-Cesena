<?php
session_start();

$document_root = $_SERVER['DOCUMENT_ROOT'];
$data = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $data['dbUserName'];
$password = $data['dbPassword'];
$dbname = $data['dbName'];

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// check if email already exists
$sql = "SELECT email
        FROM utenti
        WHERE email = ?";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $_POST['email']);
$stmt->execute();
$stmt->store_result();

if ($stmt->num_rows > 0) {
    $stmt->close();
    $conn->close();
    
    echo "<script>
            alert('Esiste già un utente con questa email!');
            window.location.href='/new_account.html';
          </script>";
}


// Insert the new user into the utenti table
$sql = "INSERT INTO utenti (email, nome, cognome, telefono, via, numeroCivico)
        VALUES (?, ?, ?, ?, ?, ?)";
$stmt = $conn->prepare($sql);
$stmt->bind_param("ssssss", $_POST['email'], $_POST['name'], $_POST['surname'], $_POST['phone'], $_POST['street'], $_POST['street_number']);
$stmt->execute();

echo "user created";

$stmt->close();
$conn->close();

header("Location: /homepage.php");
?>
