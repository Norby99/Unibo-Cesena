<?php
session_start();

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

$sql = "SELECT oraInizio, oraFine FROM fasce_orarie";
$result = $conn->query($sql);
$fasce_orarie = array();
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $fasce_orarie[] = $row;
    }
}
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
        <form action="/reserve_table_process.php" method="post">
            <div class="form-group">
                <label for="date">Giorno</label>
                <input type="date" class="form-control" id="date" name="date" value="<?php echo date('Y-m-d'); ?>" required>
            </div>
            <div class="form-group">
                <label for="time">Fascia oraria</label>
                <select class="form-control" id="time" name="time" required>
                    <?php
                    foreach($fasce_orarie as $fascia_oraria) {
                        echo '<option value="' . $fascia_oraria['oraInizio'] . '-' . $fascia_oraria['oraFine'] . '">' . $fascia_oraria['oraInizio'] . ' - ' . $fascia_oraria['oraFine'] . '</option>';
                    }
                    ?>
                </select>
            </div>
            <button type="submit" class="btn btn-primary">Seleziona tavolo</button>
        </form>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
