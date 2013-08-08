<?php

define("HOST", "xxxxxxxx"); // The host you want to connect to.  //127.0.0.1
define("USER", "xxxxxxx"); // The database username.
define("PASSWORD", "xxxxxxxx"); // The database password. 
define("DATABASE", "xxxxxxx"); // The database name.
define("PORT", "3306"); // The database name.
 
$mysqli = new mysqli(HOST, USER, PASSWORD, DATABASE, PORT);
// If you are connecting via TCP/IP rather than a UNIX socket remember to add the port number as a parameter.

?>