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
  function_tab[0] = printTemperature;
  function_tab[1] = printHumidity;
  function_tab[2] = printPressure;
  
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
//        Serial.println(String("wynik if ") + (int)(mask & *measurementTypes));
//        Serial.println(String("maska ") + (int)mask);
//        Serial.println(String("datatype ") + (int)(*measurementTypes));
      }
      mask <<= 1;
    }
  }
  Serial.println();
  
//  int* measData = reinterpret_cast<int*>(dht11.data + 1);
//  Serial.print(*measData/100.); Serial.println(F("C"));
//  Serial.print(*(measData + 1)); Serial.println(F("%"));
//  measData = reinterpret_cast<int*>(bmp180.data + 1);
//  Serial.print(*measData/10.); Serial.println(F("hPa"));
//  measData = reinterpret_cast<int*>(mpl3115a2.data + 1);
//  Serial.print(*measData/10.); Serial.println(F("hPa"));
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

void printTemperature(int temp)
{
  Serial.print(temp/100.); Serial.println(F("C"));
}

void printHumidity(int humidity)
{
  Serial.print(humidity); Serial.println(F("%"));
}

void printPressure(int pressure)
{
  Serial.print(pressure/10.); Serial.println(F("hPa"));
}
