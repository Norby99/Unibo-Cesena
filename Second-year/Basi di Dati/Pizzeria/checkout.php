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

$sql = "SELECT *
        FROM pizze
        WHERE nome = ?";

$stmt = $conn->prepare($sql);

// Calculating total price
$totalPrice = 0;
if (isset($_SESSION['basket']) && is_array($_SESSION['basket'])) {
    foreach ($_SESSION['basket'] as $pizza_basket) {
        $stmt->bind_param('s', $pizza_basket);
        $stmt->execute();
        $result = $stmt->get_result();
        $pizza = $result->fetch_assoc();
        $totalPrice += $pizza['prezzo'];
    }
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
    <title>Completa Ordine</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <?php include_once 'html_snippets/navbar.php'; create_navbar(); ?>
    <div class="container">
        <h1>Completa Ordine</h1>

        <form method="post" action="/scripts/checkout_confirm.php">
            <h3>Tipo di consegna</h3>
            <div class="form-group">
                <label for="delivery">Consegna a domicilio</label>
                <input type="radio" name="delivery" value="delivery" id="delivery">
                <label for="pickup">Ritiro</label>
                <input type="radio" name="delivery" value="pickup" id="pickup" checked>
            </div>

            <div class="form-group" id="address-group" style="display: none;">
                <label for="address">Indirizzo</label>
                <input type="text" name="address" id="address">
            </div>

            <div class="form-group">
                <label for="notes">Note</label>
                <textarea name="notes" id="notes" cols="30" rows="10"></textarea>
            </div>

            <div class="form-group" id="address-group">
                <label for="delivery-day">Giorno di consegna</label>
                <input type="date" name="delivery-day" id="delivery-day" min="<?php echo date('Y-m-d'); ?>" required>
            </div>

            <div class="form-group" id="address-group">
                <label for="delivery-time">Ora di consegna</label>
                <select name="delivery-time" id="delivery-time" required>
                    <?php
                    foreach ($fasce_orarie as $fascia_oraria) {
                        echo '<option value="' . $fascia_oraria['oraInizio'] . '">' . $fascia_oraria['oraInizio'] . ' - ' . $fascia_oraria['oraFine'] . '</option>';
                    }
                    ?>
                </select>
            </div>

            <button type="submit" class="btn btn-primary">Finalizza</button>
        </form>
    </div>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
    <script>
        const deliveryRadio = document.getElementById('delivery');
        const addressGroup = document.getElementById('address-group');

        deliveryRadio.addEventListener('change', function() {
            if (this.checked) {
                addressGroup.style.display = 'block';
                document.getElementById('address').required = true;
            } else {
                addressGroup.style.display = 'none';
                document.getElementById('address').required = false;
            }
        });

        const pickupRadio = document.getElementById('pickup');
        pickupRadio.addEventListener('change', function() {
            if (this.checked) {
                addressGroup.style.display = 'none';
                document.getElementById('address').required = false;
            }
        });
    </script>
</body>
