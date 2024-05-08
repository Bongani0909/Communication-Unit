//insertion
<?php
// Retrieve JSON data sent from ESP8266
$jsonData = file_get_contents('php://input');
 
// Decode JSON data
$data = json_decode($jsonData, true);
 
// Database connection parameters
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "meter_db";
 
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
 
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
 
// Check if the meter_flowrate or meter_voltage field exists in the JSON data
if (isset($data['meter_flowrate'])) {
    // Water meter data
    $meter_id = mysqli_real_escape_string($conn, $data['meter_id']);
    $meter_flowrate = mysqli_real_escape_string($conn, $data['meter_flowrate']);
    $meter_consumption = mysqli_real_escape_string($conn, $data['meter_consumption']);
    $meter_state = mysqli_real_escape_string($conn, $data['meter_state']);
    $time = mysqli_real_escape_string($conn, $data['time']);
    $date = mysqli_real_escape_string($conn, $data['date']);
 
    // Prepare and bind SQL statement to insert water meter data
    $stmt = $conn->prepare("INSERT INTO watermeter (meter_id, meter_flowrate, meter_consumption, meter_state, time, date)
                            VALUES (?, ?, ?, ?, ?, ?)");
    $stmt->bind_param("ssssss", $meter_id, $meter_flowrate, $meter_consumption, $meter_state, $time, $date);
 
    // Execute SQL statement
    if ($stmt->execute()) {
        echo json_encode(array("message" => "Water meter data inserted successfully"));
    } else {
        echo json_encode(array("error" => "Error inserting water meter data"));
    }
    $stmt->close();
} elseif (isset($data['meter_voltage'])) {
    // Electricity meter data
    $meter_id = mysqli_real_escape_string($conn, $data['meter_id']);
    $meter_voltage = mysqli_real_escape_string($conn, $data['meter_voltage']);
    $meter_consumption = mysqli_real_escape_string($conn, $data['meter_consumption']);
    $meter_state = mysqli_real_escape_string($conn, $data['meter_state']);
    $time = mysqli_real_escape_string($conn, $data['time']);
    $date = mysqli_real_escape_string($conn, $data['date']);
 
    // Prepare and bind SQL statement to insert electricity meter data
    $stmt = $conn->prepare("INSERT INTO elecmeter (meter_id, meter_voltage, meter_consumption, meter_state, time, date)
                            VALUES (?, ?, ?, ?, ?, ?)");
    $stmt->bind_param("ssssss", $meter_id, $meter_voltage, $meter_consumption, $meter_state, $time, $date);
 
    // Execute SQL statement
    if ($stmt->execute()) {
        echo json_encode(array("message" => "Electricity meter data inserted successfully"));
    } else {
        echo json_encode(array("error" => "Error inserting electricity meter data"));
    }
    $stmt->close();
} else {
    echo json_encode(array("error" => "Invalid JSON data format"));
}
 
// Close database connection
$conn->close();
?>
 