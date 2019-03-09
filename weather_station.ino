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
void printTemperatureOled(int temp);
void printHumidityOled(int humidity);
void printPressureOled(int pressure);

void printTemperature(int temp);
void printHumidity(int humidity);
void printPressure(int pressure);

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

SensorAdapter* SensorTab[3];
void (*function_tab[8])(int);

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Begin"));
    
  SensorTab[0] = &dht11;
  SensorTab[1] = &bmp180;
  SensorTab[2] = &mpl3115a2;
  
  SensorTab[0]->begin();
  SensorTab[1]->begin();
  SensorTab[2]->begin();

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
  delay(5000);
}

void takeMeasurement()
{
  SensorTab[0]->takeMeasurement();
  SensorTab[1]->takeMeasurement();
  SensorTab[2]->takeMeasurement();
}

void printSerial()
{
  function_tab[0] = printTemperature;
  function_tab[1] = printHumidity;
  function_tab[2] = printPressure;
  for(int i = 0; i < 3; i++)
  {
    char* measurementTypes = reinterpret_cast<char*>(SensorTab[i]->getData());
    char mask = 0b00000001;
    int* dataPtr = reinterpret_cast<int*>(measurementTypes + 1);
    
    for(int j = 0; j < 8; j++)
    {
      if(mask & *measurementTypes)
      {
        function_tab[j](*dataPtr);
        dataPtr++;
      }
      mask <<= 1;
    }
  }
}

void printOled()
{
  
  function_tab[0] = printTemperatureOled;
  function_tab[1] = printHumidityOled;
  function_tab[2] = printPressureOled;
  static int a = 0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  for(int i = 0; i < 3; i++)
  {
    char* measurementTypes = reinterpret_cast<char*>(SensorTab[i]->getData());
    char mask = 0b00000001;
    int* dataPtr = reinterpret_cast<int*>(measurementTypes + 1);
    
    for(int j = 0; j < 8; j++)
    {
      if(mask & *measurementTypes)
      {
        function_tab[j](*dataPtr);
        dataPtr++;
      }
      mask <<= 1;
    }
  
  }
  display.println(a++);
  display.display();
}

void printTemperature(int temp)
{
  Serial.print(temp/100., 1); Serial.println(F("C"));
}

void printHumidity(int humidity)
{
  Serial.print(humidity); Serial.println(F("%"));
}

void printPressure(int pressure)
{
  Serial.print(pressure/10., 1); Serial.println(F("hPa"));
}

void printTemperatureOled(int temp)
{
  display.print(temp/100., 1); display.println(F("C"));
}

void printHumidityOled(int humidity)
{
  display.print(humidity); display.println(F("%"));
}

void printPressureOled(int pressure)
{
  display.print(pressure/10., 1); display.println(F("hPa"));
}
