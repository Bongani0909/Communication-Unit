<?php

$host = "localhost";
$username = "root";
$password = "";
$dbname = "meter_db";

$conn = mysqli_connect($host, $username, $password, $dbname);

if($conn){
    echo "DB connected!";
}else{

    die("connection failed:" . mysqli_connect_error());
}

?>
