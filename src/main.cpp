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
#include "PixelArtByteArrays.h"
#define PIN  13
#define MATRIX_WIDTH 14
#define MATRIX_HEIGHT 14


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
int staticTime = 5;
int sequenceSelection = 0; // 0 = custom (0), 1 = save 1, etc.
uint16_t sequence0BitVector = 0b0000111111111111;
uint16_t sequence1BitVector = 0b0000000000010111;
uint16_t sequence2BitVector = 0b0000111111100000;
uint16_t sequence3BitVector = 0b0000000000000001;


// General Function Prototypes
void printWiFiStatus();
uint16_t updateBitVector();
void staticImage(const uint16_t image[]);
void drawImage(const uint16_t image[]);

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
void ProcessItemSel4();
void ProcessItemSel5();
void ProcessItemSel6();
void ProcessItemSel7();
void ProcessItemSel8();
void ProcessItemSel9();
void ProcessItemSel10();
void ProcessItemSel11();

// Animations
void animation0();
void animation1();
void animation2();
void animation3();
void animation4();
void animation5();
void animation6();
void animation7();
void animation8();
void animation9();
void animation10();
void animation11();

// Second Core stuff
TaskHandle_t matrixTask;
void secondCoreTask(void * pvParameters);

void setup()
{
  Serial.begin(115200);

  // Rather than try to do complex time management, I'm just gonna assign everything
  // LED matrix related to the second core.
  xTaskCreatePinnedToCore(secondCoreTask, "Matrix Management", 10000, NULL, 1, &matrixTask, 1);

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
  server.on("/ITEM_SEL_4", ProcessItemSel4);
  server.on("/ITEM_SEL_5", ProcessItemSel5);
  server.on("/ITEM_SEL_6", ProcessItemSel6);
  server.on("/ITEM_SEL_7", ProcessItemSel7);
  server.on("/ITEM_SEL_8", ProcessItemSel8);
  server.on("/ITEM_SEL_9", ProcessItemSel9);
  server.on("/ITEM_SEL_10", ProcessItemSel10);
  server.on("/ITEM_SEL_11", ProcessItemSel11);

  // Begin server
  server.begin();
}

void loop()
{
  // Everything matrix related is in secondCoreTask()

  // Must be called repeatedly! Webpage needs instructions
  server.handleClient();
}

void secondCoreTask(void * pvParameters)
{
  // Matrix startup
  matrix.begin();
  matrix.clear();
  matrix.setTextWrap(false);
  matrix.setTextColor(RED);
  matrix.setBrightness(brightness);
  matrix.show();

  // Startup flashing
  for (int i = 0; i < 20; i++)
  {
    matrix.drawPixel(0, 0, GREEN); matrix.show();
    delay(250);
    matrix.drawPixel(0, 0, BLACK); matrix.show();
    delay(250);
  }

  uint16_t currentBitVector;
  // Cycle through items (animations/images)
  // This is REALLY garbage code, I'm so sorry, but It's 3am and I graduate tomorrow. Some of the earlier code is better
  while(true)
  {
    currentBitVector = updateBitVector();
    // Attempt to play items
    if (((currentBitVector >> 0) & 1) == 1) // ((currentBitVector >> n) & 1) == 1 checks if nth bit is 1
      animation0();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 1) & 1) == 1)
      animation1();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 2) & 1) == 1)
      animation2();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 3) & 1) == 1)
      animation3();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 4) & 1) == 1)
      animation4();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 5) & 1) == 1)
      animation5();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 6) & 1) == 1)
      animation6();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 7) & 1) == 1)
      animation7();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 8) & 1) == 1)
      animation8();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 9) & 1) == 1)
      animation9();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 10) & 1) == 1)
      animation10();

    currentBitVector = updateBitVector();
    if (((currentBitVector >> 11) & 1) == 1)
      animation11();
  }
}

// Animation Functions
void animation0()
{
  staticImage(UCF_img);
}

void animation1()
{
  staticImage(pegasus_img);
}

void animation2()
{
  staticImage(NASA_img);
}

void animation3()
{
  staticImage(creeper_img);
}

void animation4()
{
  staticImage(github_img);
}

void animation5()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 25*7; i++) // the string is 25 chars
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("UCF Computer Science 2022"));
    if( --x < -160 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation6()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 10*7; i++)
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("Hi Mom!!!"));
    if( --x < -58 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation7()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 50*7; i++) 
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("Graduating with a B.S. (Bachelor's of StackOverflow)"));
    if( --x < -339 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation8()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 28*7; i++) 
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("sudo apt-get install diploma"));
    if( --x < -180 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation9()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 22*7; i++) 
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("I still can't exit vim"));
    if( --x < -140 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation10()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 32*7; i++) 
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("Robert'); DROP TABLE Students;--"));
    if( --x < -208 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(40);
  }
  delay(2000);
}

void animation11()
{
  int x = matrix.width();
  matrix.setTextColor(textColors[random(7)]);
  for (int i = 0; i < 38*7; i++) 
  {
    matrix.fillScreen(0);    //Turn off all the LEDs
    matrix.setCursor(x, 3);
    matrix.print(F("Looking for work! LinkedIn: GavinTryz"));
    if( --x < -268 )
    {
      x = matrix.width();
    }
    matrix.show();
    delay(45);
  }
  delay(2000);
}

// Matrix Helper Functions
void drawImage(const uint16_t image[])
{
  matrix.clear();
  for (int i = 0; i < MATRIX_WIDTH*MATRIX_HEIGHT; i++)
  {
    matrix.drawPixel(i%MATRIX_HEIGHT, i/MATRIX_HEIGHT, image[i]);
  }
  matrix.show();
}

void staticImage(const uint16_t image[])
{
  drawImage(image);
  if (staticTime != 35565)
    delay(staticTime*1000);
  else
    while(staticTime == 35565){}
}

uint16_t updateBitVector()
{
  switch (sequenceSelection)
  {
  case 0:
    return sequence0BitVector;
  case 1:
    return sequence1BitVector;
  case 2:
    return sequence2BitVector;
  case 3:
    return sequence3BitVector;
  default:
    return 0;
  }
  delay(10);
}

// Wireless Helper Functions
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
  strcat(XML, "<ITEM_SEL_4_STATUS>"); strcat(XML, (((sequence0BitVector >> 4) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_4_STATUS>\n");
  strcat(XML, "<ITEM_SEL_5_STATUS>"); strcat(XML, (((sequence0BitVector >> 5) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_5_STATUS>\n");
  strcat(XML, "<ITEM_SEL_6_STATUS>"); strcat(XML, (((sequence0BitVector >> 6) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_6_STATUS>\n");
  strcat(XML, "<ITEM_SEL_7_STATUS>"); strcat(XML, (((sequence0BitVector >> 7) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_7_STATUS>\n");
  strcat(XML, "<ITEM_SEL_8_STATUS>"); strcat(XML, (((sequence0BitVector >> 8) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_8_STATUS>\n");
  strcat(XML, "<ITEM_SEL_9_STATUS>"); strcat(XML, (((sequence0BitVector >> 9) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_9_STATUS>\n");
  strcat(XML, "<ITEM_SEL_10_STATUS>"); strcat(XML, (((sequence0BitVector >> 10) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_10_STATUS>\n");
  strcat(XML, "<ITEM_SEL_11_STATUS>"); strcat(XML, (((sequence0BitVector >> 11) & 1) == 1 ? "1" : "0")); strcat(XML, "</ITEM_SEL_11_STATUS>\n");

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

  matrix.setBrightness(brightness);
  matrix.show();

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

void ProcessItemSel4()
{
  Serial.println("Item selection 4 pressed!");
  sequence0BitVector ^= 1UL << 4; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel5()
{
  Serial.println("Item selection 5 pressed!");
  sequence0BitVector ^= 1UL << 5; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel6()
{
  Serial.println("Item selection 6 pressed!");
  sequence0BitVector ^= 1UL << 6; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel7()
{
  Serial.println("Item selection 7 pressed!");
  sequence0BitVector ^= 1UL << 7; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel8()
{
  Serial.println("Item selection 8 pressed!");
  sequence0BitVector ^= 1UL << 8; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel9()
{
  Serial.println("Item selection 9 pressed!");
  sequence0BitVector ^= 1UL << 9; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel10()
{
  Serial.println("Item selection 10 pressed!");
  sequence0BitVector ^= 1UL << 10; 
  server.send(TIMEOUT, "text/plain", "");
}

void ProcessItemSel11()
{
  Serial.println("Item selection 11 pressed!");
  sequence0BitVector ^= 1UL << 11; 
  server.send(TIMEOUT, "text/plain", "");
}









