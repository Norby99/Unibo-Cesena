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

// checking if there are not more than 10 reservations for the selected date and time
$sql = "SELECT COUNT(*) AS count
        FROM prenotazioni_tavolo pt
        WHERE pt.dataPrenotazione = ?
        AND pt.oraInizio = ?;";
$stmt = $conn->prepare($sql);
$stmt->bind_param("ss", $_POST['date'], $_POST['time']);
$stmt->execute();
$result = $stmt->get_result();
$count = $result->fetch_assoc()['count'];
if($count >= 10) {
    echo '  <script>
            alert("Non è possibile prenotare più di 10 tavoli per la stessa data e fascia oraria.");
            window.location.href = "/reserve_table_date.php";
            </script>';
    exit();
}

// checking if the user has already reserved a table for the selected date and time
$sql = "SELECT COUNT(*) AS count
        FROM prenotazioni_tavolo pt
        WHERE pt.idUtente = ?
        AND pt.dataPrenotazione = ?
        AND pt.oraInizio = ?;";
$stmt = $conn->prepare($sql);
$stmt->bind_param("sss", $_SESSION['id'], $_POST['date'], $_POST['time']);
$stmt->execute();
$result = $stmt->get_result();
$count = $result->fetch_assoc()['count'];
if($count > 0) {
    echo '  <script>
            alert("Hai già prenotato un tavolo per la stessa data e fascia oraria.");
            window.location.href = "/reserve_table_date.php";
            </script>';
    exit();
}

// selecting tables that are not reserved in the selected date and time
$sql = "SELECT t.idTavolo, t.posizione, t.numeroPersone
        FROM tavoli t
        WHERE t.idTavolo NOT IN (
            SELECT pt.idTavolo
            FROM prenotazioni_tavolo pt
            WHERE pt.dataPrenotazione = ? AND pt.oraInizio = ?
        );";

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
    <title>Prenota tavolo</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <?php include 'html_snippets/navbar.php'; create_navbar(); ?>
    <div class="container">
        <h1>Prenota tavolo</h1>
        <form action="scripts/reserve_table.php" method="post">
            <div class="form-group">
                <label for="table">Tavolo</label>
                <select class="form-control" id="table" name="table" required>
                    <?php
                    foreach($tables as $table) {
                        echo '<option value="' . $table['idTavolo'] . '">' . $table['posizione'] . ' - ' . $table['numeroPersone'] . " posti" . '</option>';
                    }
                    echo '<input type="hidden" name="date" value="' . $_POST['date'] . '">';
                    echo '<input type="hidden" name="time" value="' . $_POST['time'] . '">';
                    ?>
                </select>
            </div>
            <button type="submit" class="btn btn-primary">Prenota</button>
        </form>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
