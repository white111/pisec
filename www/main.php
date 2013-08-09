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

