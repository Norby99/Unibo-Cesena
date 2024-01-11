<?php
$document_root = $_SERVER['DOCUMENT_ROOT'];
$setup = json_decode(file_get_contents($document_root . '/setup.json'), true);

$servername = 'localhost';
$username = $setup['dbUserName'];
$password = $setup['dbPassword'];
$dbname = $setup['dbName'];

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$email = $_POST['email'];

$stmt = $conn->prepare("SELECT * FROM utenti WHERE email = ?");
$stmt->bind_param("s", $email);

$stmt->execute();

$result = $stmt->get_result();

if ($result->num_rows > 0) {
    session_start();
    $user = $result->fetch_assoc();
    $_SESSION['email'] = $user['email'];
    $_SESSION['name'] = $user['nome'];
    $_SESSION['surname'] = $user['cognome'];
    $_SESSION['phone'] = $user['telefono'];
    $_SESSION['isClient'] = $user['cliente'];
    $_SESSION['id'] = $user['idUtente'];
    $_SESSION['basket'] = array();
    header('Location: /homepage.php');
} else {
    echo "<script>
            alert('Non esiste alcun utente con questa mail.');
            window.location.href='/index.html';
          </script>";
}

$stmt->close();
$conn->close();
?>