//get 
<?php
// Database connection parameters
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "designwebdb";
 
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
 
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
 
// Query to fetch the latest water meter state
$sqlWater = "SELECT meter_state FROM watermeter ORDER BY meter_id DESC LIMIT 1";
$resultWater = $conn->query($sqlWater);
 
// Query to fetch the latest electricity meter state
$sqlElectricity = "SELECT meter_state FROM elecmeter ORDER BY meter_id DESC LIMIT 1";
$resultElectricity = $conn->query($sqlElectricity);
 
// Initialize variables to store meter states
$waterMeterState = "N/A";
$electricityMeterState = "N/A";
 
// Check if there is any result for water meter state
if ($resultWater && $resultWater->num_rows > 0) {
    $rowWater = $resultWater->fetch_assoc();
    $waterMeterState = $rowWater["meter_state"];
}
 
// Check if there is any result for electricity meter state
if ($resultElectricity && $resultElectricity->num_rows > 0) {
    $rowElectricity = $resultElectricity->fetch_assoc();
    $electricityMeterState = $rowElectricity["meter_state"];
}
 
// Close database connection
$conn->close();
 
// Prepare response data
$responseData = array(
    "water_meter_state" => $waterMeterState,
    "electricity_meter_state" => $electricityMeterState
);
 
// Convert response data to JSON format
$responseJson = json_encode($responseData);
 
// Set appropriate response headers
header('Content-Type: application/json');
 
// Send JSON response to ATmegas
echo $responseJson;
?>
