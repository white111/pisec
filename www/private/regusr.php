<?php
error_reporting(E_ALL);
ini_set('display_errors', True);

include 'dbconn.php';

$username = $_POST['logon'];  
$email = $_POST['email'];  
$password = $_POST['passv'];   // HASHED BY JS 

echo "recieved : " . $username . "  " .  $email . "  " .  $password . "  ";

// Create a random salt
$random_salt = hash('sha512', uniqid(mt_rand(1, mt_getrandmax()), true));
// Create salted password (Careful not to over season)
$password = hash('sha512', $password.$random_salt);
 
echo "preparing mysql";

// Add your insert to database script here. 
// Make sure you use prepared statements!
if ( ($insert_stmt = $mysqli->prepare("INSERT INTO passwd (username, email, password, salt) VALUES (?, ?, ?, ?)")) ) { 
    echo "inserting ..";
   $insert_stmt->bind_param('ssss', $username, $email, $password, $random_salt); 
   // Execute the prepared query.
   $insert_stmt->execute();
}


?>
