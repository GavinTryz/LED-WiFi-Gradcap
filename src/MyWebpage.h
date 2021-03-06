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
<!-- This should be pasted into MyWebpage.h -->
<!-- It's only here to be easier to edit.   -->
<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

    <title>Gavin's RGB Gradcap Controls</title>
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
        height: 55px;
        outline: none;
        height: 25px;
    }
    .bodytext {
        font-family: "Verdana", "Arial", sans-serif;
        font-size: 24px;
        text-align: left;
        font-weight: normal;
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



    <!-- HTML -->
    <body style="background-color: #efefef" onload="process()">

        <header>
            <div class="navbar fixed-top">
                <div class="container">
                <div class="navtitle">Graduation Cap Controls</div>
                </div>
            </div>
        </header>

        <main class="container" style="margin-top:70px">
            <br>
            <!-- Brightness -->
            <div class="category">Brightness (<span id="brightness">255</span>)</div>
            <input type="range" class="myslider" min="0" max="255" value = "255" width = "0%" oninput="UpdateSliderBrightness(this.value)"/>
            <br>
            <br>
            <br>

            <!-- Static Image Hold Time -->
            <div class="category">Static Image Hold Time (<span id="staticImageHoldTime">5</span>)</div>
            <input type="range" class="myslider" min="0" max="300" value = "5" width = "0%" oninput="UpdateSliderTime(this.value)"/>
            <br>
            <br>
            <br>

            <!-- Sequence Selection -->
            <div class="category">Sequence Selection</div>
            <br>

            <button type="button" class = "btn" id = "seqSel0Btn" onclick="seqSel0Press()">Select</button>
            <div class="bodytext" id="seqSel0Text">Custom Sequence (select below)</div><br>

            <button type="button" class = "btn" id = "seqSel1Btn" onclick="seqSel1Press()">Select</button>
            <div class="bodytext" id="seqSel1Text">Professional Static</div><br>

            <button type="button" class = "btn" id = "seqSel2Btn" onclick="seqSel2Press()">Select</button>
            <div class="bodytext" id="seqSel2Text">Scrolling Text</div><br>

            <button type="button" class = "btn" id = "seqSel3Btn" onclick="seqSel3Press()">Select</button>
            <div class="bodytext" id="seqSel3Text">UCF Logo</div>
            <br>
            <br>

            <!-- Custom Sequence Item Selection -->
            <div class="category">Custom Sequence Item Selection</div>
            <br>

            <button type="button" class = "btn" id = "itemSel0Btn" onclick="itemSel0Press()">Toggle</button>
            <div class="bodytext" id="itemSel0Text">Static UCF</div><br>

            <button type="button" class = "btn" id = "itemSel1Btn" onclick="itemSel1Press()">Toggle</button>
            <div class="bodytext" id="itemSel1Text">Static Pegasus</div><br>

            <button type="button" class = "btn" id = "itemSel2Btn" onclick="itemSel2Press()">Toggle</button>
            <div class="bodytext" id="itemSel2Text">Static NASA</div><br>

            <button type="button" class = "btn" id = "itemSel3Btn" onclick="itemSel3Press()">Toggle</button>
            <div class="bodytext" id="itemSel3Text">Static Creeper</div><br>

            <button type="button" class = "btn" id = "itemSel4Btn" onclick="itemSel4Press()">Toggle</button>
            <div class="bodytext" id="itemSel4Text">Static GitHub</div><br>

            <button type="button" class = "btn" id = "itemSel5Btn" onclick="itemSel5Press()">Toggle</button>
            <div class="bodytext" id="itemSel5Text">Text "CS 2022"</div><br>

            <button type="button" class = "btn" id = "itemSel6Btn" onclick="itemSel6Press()">Toggle</button>
            <div class="bodytext" id="itemSel6Text">Text "Hi Mom!"</div><br>

            <button type="button" class = "btn" id = "itemSel7Btn" onclick="itemSel7Press()">Toggle</button>
            <div class="bodytext" id="itemSel7Text">Text "Bachelor's of StackOverflow"</div><br>

            <button type="button" class = "btn" id = "itemSel8Btn" onclick="itemSel8Press()">Toggle</button>
            <div class="bodytext" id="itemSel8Text">Text "sudo apt-get"</div><br>

            <button type="button" class = "btn" id = "itemSel9Btn" onclick="itemSel9Press()">Toggle</button>
            <div class="bodytext" id="itemSel9Text">Text "vim"</div><br>

            <button type="button" class = "btn" id = "itemSel10Btn" onclick="itemSel10Press()">Toggle</button>
            <div class="bodytext" id="itemSel10Text">Text "little bobby drop tables"</div><br>

            <button type="button" class = "btn" id = "itemSel11Btn" onclick="itemSel11Press()">Toggle</button>
            <div class="bodytext" id="itemSel11Text">Text "Linked In"</div><br>
        </main>

        <br>
        <footer class="foot" id = "temp">Gavin Tryzbiak - University of Central Florida, Computer Science 2022</footer>

    </body>

    <script type = "text/javascript">
        // global variable visible to all java functions
        var xmlHttp=createXmlHttpObject();

        // function to create XML object
        function createXmlHttpObject()
        {
            if(window.XMLHttpRequest)
            {
                xmlHttp=new XMLHttpRequest();
            }
            else
            {
                xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
            }
            return xmlHttp;
        }

        // Sequence Selection Buttons
        // function to handle button press from HTML code above
        // and send a processing string back to server
        // this processing string is use in the .on method
        function seqSel0Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "SEQ_SEL_0", false);
            xhttp.send(); 
        }

        function seqSel1Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "SEQ_SEL_1", false);
            xhttp.send(); 
        }

        function seqSel2Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "SEQ_SEL_2", false);
            xhttp.send(); 
        }

        function seqSel3Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "SEQ_SEL_3", false);
            xhttp.send(); 
        }

        // Item Selection Buttons
        function itemSel0Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_0", false);
            xhttp.send(); 
        }

        function itemSel1Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_1", false);
            xhttp.send(); 
        }

        function itemSel2Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_2", false);
            xhttp.send(); 
        }

        function itemSel3Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_3", false);
            xhttp.send(); 
        }

        function itemSel4Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_4", false);
            xhttp.send(); 
        }

        function itemSel5Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_5", false);
            xhttp.send(); 
        }

        function itemSel6Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_6", false);
            xhttp.send(); 
        }

        function itemSel7Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_7", false);
            xhttp.send(); 
        }

        function itemSel8Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_8", false);
            xhttp.send(); 
        }

        function itemSel9Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_9", false);
            xhttp.send(); 
        }

        function itemSel10Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_10", false);
            xhttp.send(); 
        }

        function itemSel11Press()
        {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "ITEM_SEL_11", false);
            xhttp.send(); 
        }


        // Sliders
        function UpdateSliderBrightness(value)
        {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function()
            {
                if (this.readyState == 4 && this.status == 200)
                {
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

        function UpdateSliderTime(value)
        {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function()
            {
                if (this.readyState == 4 && this.status == 200)
                {
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

        
        // Function to handle response from ESP
        function response()
        {
            var message;
            var barwidth;
            var currentsensor;
            var xmlResponse;
            var xmldoc;
            var dt = new Date();
            var color = "#e8e8e8";

            // get the xml stream
            xmlResponse=xmlHttp.responseXML;

            // Selection 0
            xmldoc = xmlResponse.getElementsByTagName("SEQ_SEL_0_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("seqSel0Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Selection 1
            xmldoc = xmlResponse.getElementsByTagName("SEQ_SEL_1_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("seqSel1Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Selection 2
            xmldoc = xmlResponse.getElementsByTagName("SEQ_SEL_2_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("seqSel2Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Selection 3
            xmldoc = xmlResponse.getElementsByTagName("SEQ_SEL_3_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("seqSel3Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green



            // Item 0
            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_0_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel0Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Item 1
            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_1_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel1Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Item 2
            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_2_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel2Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            // Item 3
            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_3_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel3Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_4_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel4Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_5_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel5Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_6_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel6Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_7_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel7Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_8_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel8Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_9_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel9Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_10_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel10Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green

            xmldoc = xmlResponse.getElementsByTagName("ITEM_SEL_11_STATUS"); // This will require code in main.cpp!
            message = xmldoc[0].firstChild.nodeValue;
            document.getElementById("itemSel11Text").style.color = (message == 0 ? "#8f0000" : "#008f0a"); // Set color to red if 0, otherwise green
        }


        // general processing code for the web page to ask for an XML steam
        // this is actually why you need to keep sending data to the page to 
        // force this call with stuff like this
        // server.on("/xml", SendXML);
        // otherwise the page will not request XML from the ESP, and updates will not happen
        function process()
        {
            if(xmlHttp.readyState==0 || xmlHttp.readyState==4)
            {
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
