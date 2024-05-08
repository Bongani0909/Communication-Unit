<?php

include('dbcon.php');

function getMeterList(){

    global $conn;

    $query = "SELECT * FROM meterrr"; 
    $query_run = mysqli_query($conn, $query);

    if($query_run){

        if(mysqli_num_rows($query_run) > 0){

            $res = mysqli_fetch_all($query_run, MYSQLI_ASSOC);
            
            $data =[
                'status' => 200,
                'message' => 'Meter List Fetched Successfully',
                'data' => $res
            ];
            header("HTTP/1.0 200 OK");
            return json_encode($data);
        }else{
            $data =[
                'status' => 404,
                'message' => 'No meters Found',
            ];
            header("HTTP/1.0 404 No meters Found");
            return json_encode($data);

        }

    }else{
        
    $data =[
        'status' => 500,
        'message' => 'Internal Server Error',
    ];
    header("HTTP/1.0 500 Internal Server Error");
    return json_encode($data);
    

    }
}
?>