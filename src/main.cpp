// LED WiFi Gradcap by Gavin Tryzbiak
// Some of this code will not be pretty. I ask that spare judgement, as I started this project
// (including wiring, soldering, code, everything) 2 days before graduation.
//
// A lot of wireless code borrowed/inspired by https://github.com/KrisKasprzak/ESP32_WebPage

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>

// Wireless Definitions
#include "Secret.h" // SSID/PASS definitions are in Secret.h. Change those.
#include "MyWebpage.h"
////#define AP_SSID "????????"
////#define AP_PASS "????????"

// Matrix Definitions
#define PIN  13
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF


// Wireless Variables
char XML[2048]; // XML array (should be larger than max expected size. This is unnecessarily huge.)
char buf[32]; // Buffer for char operations
IPAddress Actual_IP;
IPAddress PageIP(192, 168, 1, 1); // Desired IP for webpage
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
WebServer server(80);

// Matrix Variables
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// General Variables
int brightness = 255;
int sequenceSelection = 0; // 0 = custom (randomized), 1 = save 1, etc.
uint16_t customSequenceBitVector = 0xFFFF;
uint16_t sequence1BitVector = 0xFFFF;
uint16_t sequence2BitVector = 0xFFFF;
uint16_t sequence3BitVector = 0xFFFF;


// Function Prototypes
void printWiFiStatus();
void SendWebsite();
void SendXML();
void UpdateSlider();


void setup()
{
  Serial.begin(115200);

  // Wireless Setup
  // if your web page or XML are large, you may not get a call back from the web page
  // and the ESP will think something has locked up and reboot the ESP
  // not sure I like this feature, actually I kinda hate it
  // disable watch dog timer 0
  disableCore0WDT();

  // maybe disable watch dog timer 1 if needed
  //  disableCore1WDT();

  // Start server
  Serial.println("Starting server...");
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: "); Serial.println(Actual_IP);

  printWiFiStatus();

  // these calls will handle data coming back from your web page
  // this one is a page request, upon ESP getting / string the web page will be sent
  server.on("/", SendWebsite);

  // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
  // just parts of the web page
  server.on("/xml", SendXML);

  // upon ESP getting /UPDATE_SLIDER string, ESP will execute the UpdateSlider function
  // same notion for the following .on calls
  // add as many as you need to process incoming strings from your web page
  // as you can imagine you will need to code some javascript in your web page to send such strings
  // this process will be documented in the SuperMon.h web page code
  server.on("/UPDATE_SLIDER", UpdateSlider);
  // server.on("/BUTTON_0", ProcessButton_0);
  // server.on("/BUTTON_1", ProcessButton_1);

  // finally begin the server
  server.begin();

  // Matrix Setup
  matrix.begin();
  matrix.clear();
  matrix.setTextWrap(false);
}

void loop()
{
  // you main loop that measures, processes, runs code, etc.
  // note that handling the "on" strings from the web page are NOT in the loop
  // that processing is in individual functions all managed by the wifi lib

  // in my example here every 50 ms, i measure some analog sensor data (my finger dragging over the pins
  // and process accordingly
  // analog input can be from temperature sensors, light sensors, digital pin sensors, etc.
  // if ((millis() - SensorUpdate) >= 50) {
  //   //Serial.println("Reading Sensors");
  //   SensorUpdate = millis();
  //   BitsA0 = analogRead(PIN_A0);
  //   BitsA1 = analogRead(PIN_A1);

  //   // standard converion to go from 12 bit resolution reads to volts on an ESP
  //   VoltsA0 = BitsA0 * 3.3 / 4096;
  //   VoltsA1 = BitsA1 * 3.3 / 4096;

  // }

  // no matter what you must call this handleClient repeatidly--otherwise the web page
  // will not get instructions to do something
  server.handleClient();
}




// Custom functions

void printWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(ip);
}

// code to send the main web page
// PAGE_MAIN is a large char defined in MyWebpage.h
void SendWebsite()
{
  Serial.println("sending web page");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/html", PAGE_MAIN);
}

// code to send the main web page
// I avoid string data types at all cost hence all the char mainipulation code
void SendXML()
{
  // Serial.println("sending xml");

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // // send bitsA0
  // sprintf(buf, "<B0>%d</B0>\n", BitsA0);
  // strcat(XML, buf);
  // // send Volts0
  // sprintf(buf, "<V0>%d.%d</V0>\n", (int) (VoltsA0), abs((int) (VoltsA0 * 10)  - ((int) (VoltsA0) * 10)));
  // strcat(XML, buf);

  // // send bits1
  // sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  // strcat(XML, buf);
  // // send Volts1
  // sprintf(buf, "<V1>%d.%d</V1>\n", (int) (VoltsA1), abs((int) (VoltsA1 * 10)  - ((int) (VoltsA1) * 10)));
  // strcat(XML, buf);

  // // show led0 status
  // if (LED0) {
  //   strcat(XML, "<LED>1</LED>\n");
  // }
  // else {
  //   strcat(XML, "<LED>0</LED>\n");
  // }

  // if (SomeOutput) {
  //   strcat(XML, "<SWITCH>1</SWITCH>\n");
  // }
  // else {
  //   strcat(XML, "<SWITCH>0</SWITCH>\n");
  // }

  strcat(XML, "</Data>\n");
  // wanna see what the XML code looks like?
  // actually print it to the serial monitor and use some text editor to get the size
  // then pad and adjust char XML[2048]; above
  Serial.println(XML);

  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/xml", XML);
}

void UpdateSlider() {

  // many I hate strings, but wifi lib uses them...
  String t_state = server.arg("VALUE");

  // conver the string sent from the web page to an int
  ////FanSpeed = t_state.toInt();
  brightness = t_state.toInt();
  ////Serial.print("UpdateSlider"); Serial.println(FanSpeed);
  Serial.print("UpdateSlider"); Serial.println(brightness);
  // now set the PWM duty cycle
  ////ledcWrite(0, FanSpeed);
  matrix.setBrightness(brightness);


  // YOU MUST SEND SOMETHING BACK TO THE WEB PAGE--BASICALLY TO KEEP IT LIVE

  // option 1: send no information back, but at least keep the page live
  // just send nothing back
  // server.send(200, "text/plain", ""); //Send web page

  // option 2: send something back immediately, maybe a pass/fail indication, maybe a measured value
  // here is how you send data back immediately and NOT through the general XML page update code
  // my simple example guesses at fan speed--ideally measure it and send back real data
  // i avoid strings at all caost, hence all the code to start with "" in the buffer and build a
  // simple piece of data
  ////FanRPM = map(FanSpeed, 0, 255, 0, 2400);
  strcpy(buf, "");
  ////sprintf(buf, "%d", FanRPM);
  sprintf(buf, "%d", brightness);
  sprintf(buf, buf);

  // now send it back
  server.send(200, "text/plain", buf); //Send web page

}