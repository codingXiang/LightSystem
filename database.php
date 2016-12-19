<?php
  header("Content-Type:text/html; charset=utf-8");
  //user information
  $host = "140.120.13.183";
  $user = "sensor";
  $pass = "123698745";

  //database information
  $databaseName = "sensor_network";
  $tableName = "sensor_value";

  $username = $_POST['username'];

  $ad = "";
  $name = "";
  if (!empty($_GET['add'])){
    $name = $_GET['username'];
    $ad = $_GET['add'];
  }

  //Connect to mysql database
  $con = mysql_connect($host,$user,$pass);
  mysql_query("SET NAMES 'UTF8'");
  $dbs = mysql_select_db($databaseName, $con);


  if (!empty($ad)){
    $result = mysql_query("SELECT * FROM $tableName WHERE address = \"$ad\" AND username = \"$name\" ORDER BY `time` ASC");
  }else{
    $result = mysql_query("SELECT * FROM $tableName WHERE username = \"$username\" ORDER BY `time` ASC");
  }
  //Query database for data

  //store matrix
  $i=0;
  while ($row = mysql_fetch_array($result)){
    $employee[$i]=$row;
    $i++;
  }

  //echo result as json
    echo json_encode($employee);
?>
