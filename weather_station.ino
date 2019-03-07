#include <SPI.h>
#include <Wire.h>
#include "BMP180Adapter.h"
#include "MPL3115A2Adapter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT11Adapter.h"


void printSerial();
void printOled();
void takeMeasurement();

constexpr uint8_t DHTPIN = 6;
constexpr uint8_t DHTTYPE = DHT11;

constexpr uint8_t SCREEN_WIDTH = 128; // OLED display width, in pixels
constexpr uint8_t SCREEN_HEIGHT = 64; // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
constexpr uint8_t OLED_MOSI  = 11;
constexpr uint8_t OLED_CLK   = 12;
constexpr uint8_t OLED_DC    = 9;
constexpr uint8_t OLED_CS    = 8;
constexpr uint8_t OLED_RESET = 10;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

BMP180Adapter        bmp180;
MPL3115A2Adapter     mpl3115a2;
DHT11Adapter         dht11 = DHT11Adapter(DHTPIN);

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Begin"));
  dht11.begin();
  bmp180.begin();
  mpl3115a2.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    while(1){};
  }
  display.display();
}

void loop() 
{
  takeMeasurement();
  printSerial();
  printOled();
  delay(20000);
}

void takeMeasurement()
{
  dht11.takeMeasurement();
  bmp180.takeMeasurement();
  mpl3115a2.takeMeasurement();
}

void printSerial()
{
  int* measData = reinterpret_cast<int*>(dht11.data + 1);
  Serial.print(*measData/100.); Serial.println(F("C"));
  Serial.print(*(measData + 1)); Serial.println(F("%"));
  measData = reinterpret_cast<int*>(bmp180.data + 1);
  Serial.print(*measData/10.); Serial.println(F("hPa"));
  measData = reinterpret_cast<int*>(mpl3115a2.data + 1);
  Serial.print(*measData/10.); Serial.println(F("hPa"));
}

void printOled()
{
  static int a = 0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  int* measData = reinterpret_cast<int*>(dht11.data + 1);
  display.print(*measData/100.); display.println(F("C"));
  display.print(*(measData + 1)); display.println(F("%"));
  measData = reinterpret_cast<int*>(bmp180.data + 1);
  display.print(*measData/10.); display.println(F("hPa"));
  measData = reinterpret_cast<int*>(mpl3115a2.data + 1);
  display.print(*measData/10.); display.println(F("hPa"));
  display.println(a++);
  display.display();
}
