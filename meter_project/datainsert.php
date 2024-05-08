<?php
// Database connection parameters
$servername = "localhost";
$username = "root";
$password ="";
$database = "meter_db";
$conn = mysqli_connect($servername ,$username ,$password ,$database);

// Check if the required parameters are present in the GET request
if (isset($_GET['userid']) && isset($_GET['water']) && isset($_GET['electricity']) && isset($_GET['balance'])) {
    // Retrieve the values from the GET request
    $userID = $_GET['userid'];
    $water = $_GET['water'];
    $electricity = $_GET['electricity'];
    $balance = $_GET['balance'];
    
    // Perform any necessary data processing here

    // Create connection
    $conn = new mysqli($servername, $username, $password, $database);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Prepare SQL statement to insert data into the database table
    $sql = "INSERT INTO meterrr (userid, water, electricity, balance) VALUES ('$userID', '$water', '$electricity', '$balance')";

    // Execute SQL statement
    if ($conn->query($sql) === TRUE) {
        echo "Data inserted successfully into the database.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // Close database connection
    $conn->close();
} else {
    // If any required parameter is missing, return an error message
    echo "Error: Required parameters are missing.";
}
?>
