<?php
error_reporting(E_ALL);
ini_set('display_errors', True);

include 'funcs.php';
include 'dbconn.php';

sec_session_start();

if(login_check($mysqli) == true) 
    {
 
    }
else
    {
     header('Location:index.html');     
    }
?>

<html>
<head>
<title>piSec</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<link href="styl.css" media="screen" rel="stylesheet" type="text/css" />

 <script type="text/javascript" src="sha512.js"></script>
 <script type="text/javascript" src="jquery-1.2.6.min.js"></script> 
 <script type="text/javascript" src="privfunc.js"></script> 
</head>

<body onload ="javascript:chmakeOutPutTab();">
  
    <div id="state">  </div>
    
<center>
<p>  <br> Arm status : </br> </p> 
<div data-aktval="n" class="statusparams" id="par0" onclick="javascript:activate(this.id);">    </div>
<br /> <br />
    
<p>  <br> Violation status : </br> </p> 
<div   data-aktval="n" class="statusparams" id="par1" onclick="javascript:activate(this.id);">   </div>

<br /><br />
    
<table>   
    
<tr id="outputsrow">    
</tr>

</table>

</center>

</body>
</html>

