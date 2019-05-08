#include <SPI.h>
#include <Wire.h>
#include "BMP180Wrapper.h"
#include "BMP280Wrapper.h"
#include "MPL3115A2Wrapper.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT11Wrapper.h"


void printSerial();
void printOled();
void printDHT11();
void printDHT22();
void printBMP180();
void printBMP280();
void printMPL3115A2();

void takeMeasurement();
void printTemperatureOled(int temp);
void printHumidityOled(int humidity);
void printPressureOled(int pressure);

void printTemperature(int temp);
void printHumidity(int humidity);
void printPressure(int pressure);

namespace
{
constexpr uint8_t DHT11PIN = 6;
constexpr uint8_t DHT22PIN = 2;

constexpr uint8_t DHT11TYPE = 11;
constexpr uint8_t DHT22TYPE = 22;

constexpr uint8_t SCREEN_WIDTH = 128; // OLED display width, in pixels
constexpr uint8_t SCREEN_HEIGHT = 64; // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
constexpr uint8_t OLED_MOSI  = 11;
constexpr uint8_t OLED_CLK   = 12;
constexpr uint8_t OLED_DC    = 9;
constexpr uint8_t OLED_CS    = 8;
constexpr uint8_t OLED_RESET = 10;

constexpr char measTypes[] = {TEMPERATURE, HUMIDITY, PRESSURE};
constexpr uint8_t numAdapters = 5;
constexpr uint8_t numMeasurementsFunctions = 3;

using MeasFunction = void (*)(int);
MeasFunction measFunctions[3];
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

BMP180Wrapper        bmp180;
BMP280Wrapper        bmp280;
MPL3115A2Wrapper     mpl3115a2;
DHTWrapper           dht11 = DHTWrapper(DHT11PIN, DHT11TYPE);
DHTWrapper           dht22 = DHTWrapper(DHT22PIN, DHT22TYPE);

SensorWrapper* SensorTab[numAdapters];


void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Begin"));
    
  SensorTab[0] = &dht11;
  SensorTab[1] = &dht22;
  SensorTab[2] = &bmp180;
  SensorTab[3] = &bmp280;
  SensorTab[4] = &mpl3115a2;
  
  SensorTab[0]->begin();
  SensorTab[1]->begin();
  SensorTab[2]->begin();
  SensorTab[3]->begin();
  SensorTab[4]->begin();

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
  printDHT11();
  printDHT22();
  printBMP180();
  printBMP280();
  printMPL3115A2();
}

void takeMeasurement()
{
  SensorTab[0]->takeMeasurement();
  SensorTab[1]->takeMeasurement();
  SensorTab[2]->takeMeasurement();
  SensorTab[3]->takeMeasurement();
  SensorTab[4]->takeMeasurement();
}

void printMeasurements()
{
  for(uint8_t i = 0; i < numAdapters; i++)
  {
    auto measurementData = SensorTab[i]->getData();
    MeasurementTypePtr measType = reinterpret_cast<MeasurementTypePtr>(measurementData);
    MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(measurementData + 1);
    
    for(uint8_t j = 0; j < numMeasurementsFunctions; j++)
    {
      if(measTypes[j] & *measType)
      {
        measFunctions[j](*measData);
        measData++;
      }
    }
  }
}

void printSerial()
{
  measFunctions[0] = printTemperature;
  measFunctions[1] = printHumidity;
  measFunctions[2] = printPressure;
  printMeasurements();
}

void printOled()
{
  
  measFunctions[0] = printTemperatureOled;
  measFunctions[1] = printHumidityOled;
  measFunctions[2] = printPressureOled;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  printMeasurements();
  display.display();
}

void printDHT11()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("DHT11"));
  display.setTextSize(1);
  
  MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(SensorTab[0]->getData() + 1);
  printTemperatureOled(*(measData++));
  printHumidityOled(*measData);
  display.display();
  delay(5000);
}

void printDHT22()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("DHT22"));
  display.setTextSize(1);
  
  MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(SensorTab[1]->getData() + 1);
  printTemperatureOled(*(measData++));
  printHumidityOled(*measData);
  display.display();
  delay(5000);
}

void printBMP180()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("BMP180"));
  display.setTextSize(1);
  
  MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(SensorTab[2]->getData() + 1);
  printPressureOled(*measData);
  display.display();
  delay(5000);
}

void printBMP280()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("BMP280"));
  display.setTextSize(1);
  
  MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(SensorTab[3]->getData() + 1);
  printTemperatureOled(*(measData++));
  printPressureOled(*measData);
  display.display();
  delay(5000);
}

void printMPL3115A2()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("MPL3115A2"));
  display.setTextSize(1);
  
  MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(SensorTab[4]->getData() + 1);
  printPressureOled(*measData);
  display.display();
  delay(5000);
}

void printTemperature(int temp)
{
  Serial.print(decompressTemperature(temp), 1); Serial.println(F("C"));
}

void printHumidity(int humidity)
{
  Serial.print(humidity); Serial.println(F("%"));
}

void printPressure(int pressure)
{
  Serial.print(decompressPressure(pressure), 1); Serial.println(F("hPa"));
}

void printTemperatureOled(int temp)
{
  display.print(decompressTemperature(temp), 1); display.println(F("C"));
}

void printHumidityOled(int humidity)
{
  display.print(humidity); display.println(F("%"));
}

void printPressureOled(int pressure)
{
  display.print(decompressPressure(pressure), 1); display.println(F("hPa"));
}
