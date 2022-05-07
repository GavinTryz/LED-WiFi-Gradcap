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
#define TIMEOUT 200 // Slower pages may need a higher timeout. In ms

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
int staticTime = 30;
int sequenceSelection = 0; // 0 = custom (randomized), 1 = save 1, etc.
uint16_t sequence0BitVector = 0xFFFF;
uint16_t sequence1BitVector = 0xFFFF;
uint16_t sequence2BitVector = 0xFFFF;
uint16_t sequence3BitVector = 0xFFFF;


// General Function Prototypes
void printWiFiStatus();

// Handler Prototypes
void SendWebsite();
void SendXML();
void UpdateSliderBrightness();
void UpdateSliderTime();
void ProcessSeqSel0();
void ProcessSeqSel1();
void ProcessSeqSel2();
void ProcessSeqSel3();
void ProcessItemSel0();
void ProcessItemSel1();
void ProcessItemSel2();
void ProcessItemSel3();


void setup()
{
  Serial.begin(115200);

  // disable watch dog timer 0
  disableCore0WDT();
  // maybe disable watch dog timer 1 if needed
  // diableCore1WDT();

  Serial.println("Starting server...");
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("AP running. Web address: "); Serial.println(Actual_IP);

  printWiFiStatus();


  // Handle data coming from web page
  server.on("/", SendWebsite); // Page request, send webpage
  server.on("/xml", SendXML); // Build and send XML (data on interactables)
  server.on("/UPDATE_SLIDER_BRIGHTNESS", UpdateSliderBrightness);
  server.on("/UPDATE_SLIDER_TIME", UpdateSliderTime);
  server.on("/SEQ_SEL_0", ProcessSeqSel0);
  server.on("/SEQ_SEL_1", ProcessSeqSel1);
  server.on("/SEQ_SEL_2", ProcessSeqSel2);
  server.on("/SEQ_SEL_3", ProcessSeqSel3);
  server.on("/ITEM_SEL_0", ProcessItemSel0);
  server.on("/ITEM_SEL_1", ProcessItemSel1);
  server.on("/ITEM_SEL_2", ProcessItemSel2);
  server.on("/ITEM_SEL_3", ProcessItemSel3);

  // Begin server
  server.begin();
}

void loop()
{
  // TODO

  // Must be called repeatedly! Webpage needs instructions
  server.handleClient();
}


// Helper Functions
void printWiFiStatus()
{
  // Print the SSID of the network you're attached to:
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

// Webpage handlers
void SendWebsite()
{
  // May have to change first agument value. Large pages may need more time,
  // 200ms may not be enough for a timeout.
  server.send(TIMEOUT, "text/html", PAGE_MAIN);
}

void SendXML()
{
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // Update Statuses
  // Sequence Selection
  strcat(XML, "<SEQ_SEL_0_STATUS>"); strcat(XML, (sequenceSelection == 0 ? "1" : "0")); strcat(XML, "</SEQ_SEL_0_STATUS>\n");
  strcat(XML, "<SEQ_SEL_1_STATUS>"); strcat(XML, (sequenceSelection == 1 ? "1" : "0")); strcat(XML, "</SEQ_SEL_1_STATUS>\n");
  strcat(XML, "<SEQ_SEL_2_STATUS>"); strcat(XML, (sequenceSelection == 2 ? "1" : "0")); strcat(XML, "</SEQ_SEL_2_STATUS>\n");
  strcat(XML, "<SEQ_SEL_3_STATUS>"); strcat(XML, (sequenceSelection == 3 ? "1" : "0")); strcat(XML, "</SEQ_SEL_3_STATUS>\n");

  // Item Selection
  strcat(XML, "<ITEM_SEL_0_STATUS>"); strcat(XML, (((sequence0BitVector >> 0) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_0_STATUS>\n"); // bitVector >> n & 1 checks if the nth bit is 1
  strcat(XML, "<ITEM_SEL_1_STATUS>"); strcat(XML, (((sequence0BitVector >> 1) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_1_STATUS>\n");
  strcat(XML, "<ITEM_SEL_2_STATUS>"); strcat(XML, (((sequence0BitVector >> 2) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_2_STATUS>\n");
  strcat(XML, "<ITEM_SEL_3_STATUS>"); strcat(XML, (((sequence0BitVector >> 3) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_3_STATUS>\n");

  strcat(XML, "</Data>\n");

  //Serial.println("Sending XML...");
  //Serial.println(XML);

  // Larger pages may need processing time and a larger timeout
  server.send(TIMEOUT, "text/xml", XML);
}

void UpdateSliderBrightness()
{
  String t_state = server.arg("VALUE");
  brightness = t_state.toInt();

  if (brightness > 225) // Was probably trying to drag it to 255
    brightness = 255;

  Serial.print("Updated Brightness Slider to "); Serial.println(brightness);
  strcpy(buf, "");
  sprintf(buf, "%d", brightness);
  sprintf(buf, buf);

  server.send(TIMEOUT, "text/plain", buf);
}

void UpdateSliderTime()
{
  String t_state = server.arg("VALUE");
  staticTime = t_state.toInt();

  if (staticTime > 450) // Anything less than 450 seconds should become ~infinity
    staticTime = 65535;

  Serial.print("Updated Static Time Slider to "); Serial.println(staticTime);
  strcpy(buf, "");
  if (staticTime == 65535)
    sprintf(buf, "∞");
  else
    sprintf(buf, "%d", staticTime);
  sprintf(buf, buf);

  server.send(TIMEOUT, "text/plain", buf);
}

void ProcessSeqSel0()
{
  Serial.println("Sequence selection 0 pressed!");
  sequenceSelection = 0;
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessSeqSel1()
{
  Serial.println("Sequence selection 1 pressed!");
  sequenceSelection = 1;
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessSeqSel2()
{
  Serial.println("Sequence selection 2 pressed!");
  sequenceSelection = 2;
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessSeqSel3()
{
  Serial.println("Sequence selection 3 pressed!");
  sequenceSelection = 3;
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel0()
{
  Serial.println("Item selection 0 pressed!");
  sequence0BitVector ^= 1UL << 0; // ^= 1UL << n flips the nth bit
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel1()
{
  Serial.println("Item selection 1 pressed!");
  sequence0BitVector ^= 1UL << 1; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel2()
{
  Serial.println("Item selection 2 pressed!");
  sequence0BitVector ^= 1UL << 2; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel3()
{
  Serial.println("Item selection 3 pressed!");
  sequence0BitVector ^= 1UL << 3; 
  server.send(TIMEOUT, "text/plain", "");
}









