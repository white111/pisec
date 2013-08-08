<?php	
error_reporting(E_ALL);
ini_set('display_errors', True);

include 'funcs.php';
include 'dbconn.php';

sec_session_start(); //  custom secure way of starting a php session
 
$_SESSION["loggedIn"] = false; 

if(isset($_POST['logon'], $_POST['passv'])) 
{   
   $login = $_POST['logon']; 
   $password = $_POST['passv']; // The hashed password.
   
   if(login($login, $password, $mysqli) == true) 
   {
      // Login success
    //  echo 'Success: You have been logged in!';
      header('Location: ./main.php');
   } 
   
   else 
   {
      // Login failed
      header('Location: ./index.html');
   }
   
}

else 
{ 
   // The correct POST variables were not sent to this page.
   echo 'Invalid Request';
}    
        
        
        
        
        
?>