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

$sql = "SELECT t.idTavolo, t.posizione, u.nome, u.cognome
        FROM tavoli t
        INNER JOIN prenotazioni_tavolo pt ON t.idTavolo = pt.idTavolo
        INNER JOIN utenti u ON pt.idUtente = u.idUtente
        WHERE pt.dataPrenotazione = ?
        AND pt.oraInizio = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("ss", $_POST['date'], $_POST['time']);
$stmt->execute();
$result = $stmt->get_result();
$tables = array();
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $tables[] = $row;
    }
}

$stmt->close();
$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reserved Tables</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <?php include 'html_snippets/navbar.php'; create_navbar(); ?>
    <div class="container">
        <h1>Reserved Tables</h1>
        <table class="table">
            <thead>
                <tr>
                    <th scope="col">ID Tavolo</th>
                    <th scope="col">Position</th>
                    <th scope="col">Nome Cliente</th>
                </tr>
            </thead>
            <tbody>
                <?php
                foreach($tables as $table) {
                    echo '<tr>';
                    echo '<td>' . $table['idTavolo'] . '</td>';
                    echo '<td>' . $table['posizione'] . '</td>';
                    echo '<td>' . $table['nome'] . ' ' . $table['cognome'] . '</td>';
                    echo '</tr>';
                }
                if (count($tables) == 0) {
                    echo '<tr><td colspan="2">Non ci sono tavoli prenotati</td></tr>';
                }
                ?>
            </tbody>
        </table>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
