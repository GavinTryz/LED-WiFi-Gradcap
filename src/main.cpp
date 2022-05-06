#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "MyWebpage.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>

// Wireless Definitions
#include "Secret.h" // SSID/PASS definitions are in Secret.h. Change those.
//#define AP_SSID "????????"
//#define AP_PASS "????????"

// Matrix Definitions
#define LED_PIN  13
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF


// Wireless Variables

// Matrix Variables


// Function Prototypes

void setup()
{
  // Wireless Setup

  // Matrix Setup
}

void loop()
{

}