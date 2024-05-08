
<?php

$hostname = "localhost";
$username = "root";
$password ="";
$database = "meter_db";
$conn = mysqli_connect($hostname ,$username ,$password ,$database);

if(!$conn){
    die("connection failed: ". mysqli_connect_error());
}

echo "Database connection was successful!!!";

 $t = 27;
 $h = 55;
 $sql = "INSERT INTO meter_records(val, val2) VALUES (".$t.", ".$h.")";

 if (mysqli_query($conn, $sql)){
     echo "new record created successfully";
 }else{
     echo "Error: " . $sql . "<br" . mysqli_error($conn);

 }
/*if(isset($_POST["temperature"]) && isset($_POST["humidity"])) {

	$t = $_POST["temperature"];
	$h = $_POST["humidity"];

	$sql = "INSERT INTO dht11 (temperature, humidity) VALUES (".$t.", ".$h.")"; 

	if (mysqli_query($conn, $sql)) { 
		echo "\nNew record created successfully"; 
	} else { 
		echo "Error: " . $sql . "<br>" . mysqli_error($conn); 
	}
}*/

?>