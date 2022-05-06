// Note: this was heavily borrowed and modified from https://github.com/KrisKasprzak/ESP32_WebPage
/*


  OK, ya ready for some fun? HTML + CSS styling + javascript all in and undebuggable environment

  one trick I've learned to how to debug HTML and CSS code.

  get all your HTML code (from html to /html) and past it into this test site
  muck with the HTML and CSS code until it's what you want
  https://www.w3schools.com/html/tryit.asp?filename=tryhtml_intro

  No clue how to debug javascrip other that write, compile, upload, refresh, guess, repeat

  I'm using class designators to set styles and id's for data updating
  for example:
  the CSS class .tabledata defines with the cell will look like
  <td><div class="tabledata" id = "switch"></div></td>

  the XML code will update the data where id = "switch"
  java script then uses getElementById
  document.getElementById("switch").innerHTML="Switch is OFF";


  .. now you can have the class define the look AND the class update the content, but you will then need
  a class for every data field that must be updated, here's what that will look like
  <td><div class="switch"></div></td>

  the XML code will update the data where class = "switch"
  java script then uses getElementsByClassName
  document.getElementsByClassName("switch")[0].style.color=text_color;


  the main general sections of a web page are the following and used here

  <html>
    <style>
    // dump CSS style stuff in here
    </style>
    <body>
      <header>
      // put header code for cute banners here
      </header>
      <main>
      // the buld of your web page contents
      </main>
      <footer>
      // put cute footer (c) 2021 xyz inc type thing
      </footer>
    </body>
    <script>
    // you java code between these tags
    </script>
  </html>


*/

// note R"KEYWORD( html page code )KEYWORD"; 
// again I hate strings, so char is it and this method let's us write naturally

const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>Web Page Update Demo</title>

  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
       padding: 3px 15px;
    }
    .tabledata {
      font-size: 24px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      background-color: #00AA00;
    }
    .myslider {
      width: 100%;
      outline: none;
      height: 55px;
      
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 24px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 50px;
      margin: 0;
      padding: 10px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 50px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 20px;
    }
   .navheading {
     position: fixed;
     left: 60%;
     height: 50px;
     font-family: "Verdana", "Arial", sans-serif;
     font-size: 20px;
     font-weight: bold;
     line-height: 20px;
     padding-right: 20px;
   }
   .navdata {
      justify-content: flex-end;
      position: fixed;
      left: 70%;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: bold;
      line-height: 20px;
      padding-right: 20px;
   }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 32px;
      line-height: 50px;
      padding: 20px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 28px;
      text-align: left;
    }
  
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
    }
    .foot {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      position: relative;
      height:   30px;
      text-align: center;   
      color: #AAAAAA;
      line-height: 20px;
    }
    .container {
      max-width: 1800px;
      margin: 0 auto;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }
    
  </style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">Graduation Cap Controls</div>
            
          </div>
      </div>
    </header>
  
    <main class="container" style="margin-top:70px">
      
    </div>
    <br>
    <div class="category">Brightness (<span id="brightness"></span>)</div>
    <input type="range" class="myslider" min="0" max="255" value = "0" width = "0%" oninput="UpdateSliderBrightness(this.value)"/>
    <br>
    <br>
    <br>
    <div class="category">Static Image Hold Time (<span id="staticImageHoldTime"></span>)</div>
    <input type="range" class="myslider" min="0" max="500" value = "0" width = "0%" oninput="UpdateSliderTime(this.value)"/>
    <br>
    <br>
    <br>
    <div class="category">Sequence Selection</div>
    <br>
    <input type="radio" id="seqSel0" name="sequenceSelection" value="0" onclick="seqSelect0()">
  	<label for="seqSel0" class="bodytext">Custom Sequence (select below)</label><br><br>
    
    <input type="radio" id="seqSel1" name="sequenceSelection" value="1" onclick="seqSelect1()">
  	<label for="seqSel1" class="bodytext">Sequence Save 1</label><br><br>
    
    <input type="radio" id="seqSel2" name="sequenceSelection" value="2" onclick="seqSelect2()">
  	<label for="seqSel2" class="bodytext">Sequence Save 2</label><br><br>
    
    <input type="radio" id="seqSel3" name="sequenceSelection" value="3" onclick="seqSelect3()">
  	<label for="seqSel3" class="bodytext">Sequence Save 3</label><br><br>
    <br>
    <br>
    <br>
    <div class="category">Custom Sequence Item Selection</div>
    <br>
    <input type="checkbox" id="itemSel0" name="itemSelection" value="0" onclick="itemSelect0()">
  	<label for="itemSel0" class="bodytext">Static UCF Football</label><br><br>
    
    <input type="checkbox" id="itemSel1" name="itemSelection" value="1" onclick="itemSelect1()">
  	<label for="itemSel1" class="bodytext">Static Pegasus</label><br><br>
    
    <input type="checkbox" id="itemSel2" name="itemSelection" value="2" onclick="itemSelect2()">
  	<label for="itemSel2" class="bodytext">Static NASA</label><br><br>
    
    <input type="checkbox" id="itemSel3" name="itemSelection" value="3" onclick="itemSelect3()">
  	<label for="itemSel3" class="bodytext">Scrolling "Hi Mom!"</label><br><br>
    
    <br>
    <br>
    <br>
    <div class="bodytext">LED </div>
    <button type="button" class = "btn" id = "btn0" onclick="ButtonPress0()">Toggle</button>
    </div>
    <br>
    <div class="bodytext">Switch</div>
    <button type="button" class = "btn" id = "btn1" onclick="ButtonPress1()">Toggle</button>
    </div>
    <br>
    <br>
    
    <br>
    <br>
  </main>

  <footer div class="foot" id = "temp" >Gavin Tryzbiak - University of Central Florida, Computer Science 2022</div></footer>
  
  </body>


  <script type = "text/javascript">
  
    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();

    // function to create XML object
    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }






	// function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function seqSelect0() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "SEQ_SEL_0", false);
      xhttp.send();
    }
    
    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function seqSelect1() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "SEQ_SEL_1", false);
      xhttp.send();
    }
    
    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function seqSelect2() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "SEQ_SEL_2", false);
      xhttp.send();
    }
    
    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function seqSelect3() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "SEQ_SEL_3", false);
      xhttp.send();
    }






    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPress0() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
      
      xhttp.open("PUT", "BUTTON_0", false);
      xhttp.send();
    }






    
    function UpdateSliderBrightness(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("brightness").innerHTML=this.responseText;
        }
      }
      // this syntax is really weird the ? is a delimiter
      // first arg UPDATE_SLIDER is use in the .on method
      // server.on("/UPDATE_SLIDER", UpdateSlider);
      // then the second arg VALUE is use in the processing function
      // String t_state = server.arg("VALUE");
      // then + the controls value property
      xhttp.open("PUT", "UPDATE_SLIDER_BRIGHTNESS?VALUE="+value, true);
      xhttp.send();
    }
    
    function UpdateSliderTime(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("staticImageHoldTime").innerHTML=this.responseText;
        }
      }
      // this syntax is really weird the ? is a delimiter
      // first arg UPDATE_SLIDER is use in the .on method
      // server.on("/UPDATE_SLIDER", UpdateSlider);
      // then the second arg VALUE is use in the processing function
      // String t_state = server.arg("VALUE");
      // then + the controls value property
      xhttp.open("PUT", "UPDATE_SLIDER_TIME?VALUE="+value, true);
      xhttp.send();
    }
    
    
    
    

    // function to handle the response from the ESP
    function response(){
      var message;
      var barwidth;
      //var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
      // get host date and time
//      document.getElementById("time").innerHTML = dt.toLocaleTimeString();
//      document.getElementById("date").innerHTML = dt.toLocaleDateString();
  
      // A0
//      xmldoc = xmlResponse.getElementsByTagName("B0"); //bits for A0
//      message = xmldoc[0].firstChild.nodeValue;
      
//      if (message > 2048){
//      color = "#aa0000";
//      }
//      else {
//        color = "#0000aa";
//      }
      
      barwidth = message / 40.95;
      //document.getElementById("b0").innerHTML=message;
      //document.getElementById("b0").style.width=(barwidth+"%");
      // if you want to use global color set above in <style> section
      // other wise uncomment and let the value dictate the color
      //document.getElementById("b0").style.backgroundColor=color;
      //document.getElementById("b0").style.borderRadius="5px";
      
//      xmldoc = xmlResponse.getElementsByTagName("V0"); //volts for A0
//      message = xmldoc[0].firstChild.nodeValue;
      //document.getElementById("v0").innerHTML=message;
      //document.getElementById("v0").style.width=(barwidth+"%");
      // you can set color dynamically, maybe blue below a value, red above
      //document.getElementById("v0").style.backgroundColor=color;
      //document.getElementById("v0").style.borderRadius="5px";
  
      // A1
//      xmldoc = xmlResponse.getElementsByTagName("B1");
//      message = xmldoc[0].firstChild.nodeValue;
//      if (message > 2048){
//      color = "#aa0000";
//      }
//      else {
//        color = "#0000aa";
//      }
//      document.getElementById("b1").innerHTML=message;
//      width = message / 40.95;
//      document.getElementById("b1").style.width=(width+"%");
 //     document.getElementById("b1").style.backgroundColor=color;
      //document.getElementById("b1").style.borderRadius="5px";
      
//      xmldoc = xmlResponse.getElementsByTagName("V1");
//      message = xmldoc[0].firstChild.nodeValue;
//      document.getElementById("v1").innerHTML=message;
//      document.getElementById("v1").style.width=(width+"%");
//      document.getElementById("v1").style.backgroundColor=color;
      //document.getElementById("v1").style.borderRadius="5px";
  
      xmldoc = xmlResponse.getElementsByTagName("LED");
      message = xmldoc[0].firstChild.nodeValue;
  
      if (message == 0){
        document.getElementById("btn0").innerHTML="Turn ON";
      }
      else{
        document.getElementById("btn0").innerHTML="Turn OFF";
      }
         
      xmldoc = xmlResponse.getElementsByTagName("SWITCH");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("switch").style.backgroundColor="rgb(200,200,200)";
      // update the text in the table
      if (message == 0){
        document.getElementById("switch").innerHTML="Switch is OFF";
        document.getElementById("btn1").innerHTML="Turn ON";
        document.getElementById("switch").style.color="#0000AA"; 
      }
      else {
        document.getElementById("switch").innerHTML="Switch is ON";
        document.getElementById("btn1").innerHTML="Turn OFF";
        document.getElementById("switch").style.color="#00AA00";
      }
     }
  
    // general processing code for the web page to ask for an XML steam
    // this is actually why you need to keep sending data to the page to 
    // force this call with stuff like this
    // server.on("/xml", SendXML);
    // otherwise the page will not request XML from the ESP, and updates will not happen
    function process(){
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()",100);
    }
  
  
  </script>

</html>



)=====";
