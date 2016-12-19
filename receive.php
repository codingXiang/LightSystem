<?php

  $light = $_GET['value'];


  //user information
  $host = "140.120.54.157";
  $user = "Test2";
  $pass = "Test2";

  //database information
  $databaseName = "iot1128";
  $tableName = "light";


  //Connect to mysql database
  $con = mysql_connect($host,$user,$pass);
  $dbs = mysql_select_db($databaseName, $con);


  //Query database for data
    $result = mysql_query("insert into $tableName (value) VALUES ($light)");

  //store matrix
  if($result==1)
    echo "success";
  else
    echo "error";
?>
