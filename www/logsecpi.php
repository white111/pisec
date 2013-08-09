<?php	

/*
Copyright (C) Dominik Guz

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/



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