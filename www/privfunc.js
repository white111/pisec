
 function reloadstyle()
 {
      var params = document.getElementsByClassName('statusparams');
      
      //setup background image based on current value
               // arm  style
                   var aktval =  params[0].getAttribute("data-aktval");
                   if ( aktval == 'n')  {  params[0].setAttribute("style", "background-image: url('img/unlocked.jpg');" ); }
                   else { params[0].setAttribute("style", "background-image: url('img/locked.jpg');" ); }
               
               
               //violation style
                    var aktval =  params[1].getAttribute("data-aktval");
                   if ( aktval == 'n')  {  params[1].setAttribute("style", "background-image: url('img/burg-g.jpg');" ); }
                   else { params[1].setAttribute("style", "background-image: url('img/burg-r.jpg');" ); }
               
               
        for ($i = 2; $i < params.length; $i++)
               { 
                   var aktval =  params[$i].getAttribute("data-aktval");
                   if ( aktval == 'n')  {  params[$i].setAttribute("style", "background-image: url('img/aup.jpg');" ); }
                   else { params[$i].setAttribute("style", "background-image: url('img/adown.jpg');" ); }
               }
     
 }
 
 
 function formhash(form, password) {
   // Create a new element input, this will be out hashed password field.
   var p = document.createElement("input");
   // Add the new element to our form.
   form.appendChild(p);
   p.name = "passv";
   p.type = "hidden"
   p.value = hex_sha512(password.value);
   // Make sure the plaintext password doesn't get sent.
   password.value = "";
   // Finally submit the form.
   form.submit();
}
 
 
 function readstate()  //read state from memory in specified intervals?
 {  
     //alert( "starting read");
     //get state from memory
     
     if (window.XMLHttpRequest)
      {  xmlhttp=new XMLHttpRequest();  }
    else
      {  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP"); }
    xmlhttp.onreadystatechange=function()
      { 
      if (xmlhttp.readyState==4 && xmlhttp.status==200)
        { 
            var statestr = xmlhttp.responseText;
  	//	alert("resp = " + statestr); 
            var statetab = statestr.split("");          
            var params = document.getElementsByClassName('statusparams');
    
           // if (  (statetab.length > 0)  && (statetab.length == params.length) )  
            for ($i = 0; $i < params.length; $i++)
               {
                   params[$i].setAttribute("data-aktval", statetab[$i]);
               }
         
            reloadstyle(); //reload css bckgr
      }    
     };
     
    xmlhttp.open("POST","relioR.php",true);    
    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xmlhttp.send();  
 }
 
 
 
 function activate(parid)
 {
   //alert( "starting activation");
     /*seqnum :
      * 0 = ARM
      * 1 = DISARM
      * 2 - swap OUT0
      * ...     OUT7
      * 
      * 
      * parid =  'par' + seqnum 
      * 
      */
    if (window.XMLHttpRequest)
      {  xmlhttp=new XMLHttpRequest();  }
    else
      {  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP"); }
    xmlhttp.onreadystatechange=function()
      { 
      if (xmlhttp.readyState==4 && xmlhttp.status==200)
        { 
            var statesrc = xmlhttp.responseText;
            //alert(statesrc);
            document.getElementById("state").innerHTML = statesrc;
            
            //get saved data from memory and reload styles
            readstate();
            reloadstyle();
        }
      };
    
    //find swap state
    var parstate = "n";   
    var aktvalue = document.getElementById(parid).getAttribute("data-aktval");
    if ( aktvalue == "y") { parstate = "n"; }
    else { parstate = "y"; }
    
    //find par nr 
    var ionr = parid.charAt(3);
    
    // alert ("ionr = " + ionr + "  parstate = " + parstate);
    
    xmlhttp.open("POST","relioW.php",true);    
    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xmlhttp.send("parnum="+ionr+"&parstate="+parstate);
     
 }
 
 
 function chmakeOutPutTab()
 { 
    if (window.XMLHttpRequest)
      {  xmlhttp=new XMLHttpRequest();  }
    else
      {  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP"); }
    xmlhttp.onreadystatechange=function()
      { 
      if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {             
            //find out number of output based on state object size
            var statestr = xmlhttp.responseText;
		//alert("state = " + statestr);
            var statetab = statestr.split("");
		//alert("size = " + statetab.length);
            var outputrow = document.getElementById("outputsrow");
            var outputrowHTML = "";
           //create TD for each output, with current state from SHM
            for ($i = 2; $i < statetab.length-3; $i++)
               { //    <td   data-aktval="n" class="statusparams" id="par3" onclick="javascript:activate(this.id);">  out 2 </td>  <br/>
                   outputrowHTML += ' <td   data-aktval="' + statetab[$i] + '" class="statusparams" id="par'+$i+'" onclick="javascript:activate(this.id);">  Output no '+$i+' </td>   <br/> ';
               }
            outputrow.innerHTML = outputrowHTML;
            
            //Assign arm/violation state
            document.getElementById("par0").setAttribute("data-aktval", statetab[0]);
            document.getElementById("par1").setAttribute("data-aktval", statetab[1]);
            
            reloadstyle(); //reload css bckgr
            
            //refresh states in specified interval - default = 5 sec.
            window.setInterval(readstate, 5000);
      }    
     };
     
    xmlhttp.open("POST","relioR.php",true);    
    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xmlhttp.send();  
     
 }
 
