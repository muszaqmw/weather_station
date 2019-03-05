#include <SPI.h>
#include <Wire.h>
#include "BMP180Adapter.h"
#include "MPL3115A2Adapter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT11Adapter.h"

//void printSerial();
void printOled();
void takeMeasurement();

constexpr uint8_t DHTPIN = 6;
constexpr uint8_t DHTTYPE = DHT11;   // DHT 11

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

Adafruit_MPL3115A2 mpl = Adafruit_MPL3115A2();
Adafruit_BMP085 bmp = Adafruit_BMP085();

DHT dht(DHTPIN, DHTTYPE);

BMP180Adapter        bmp180(bmp);
MPL3115A2Adapter     mpl3115a2 = MPL3115A2Adapter(mpl);

DHT11Adapter         dht11(dht);

namespace
{
struct MeasurementsTemp
{
  struct DHT11Sensor
  {
    float temperature = 0;
    float humidity = 0;
  };
  
  struct BMP180Sensor
  {
    float temperature = 0;
    float pressure = 0;
  };

  struct MPL3115A2Sensor
  {
    float temperature = 0;
    float pressure = 0;
  };

  DHT11Sensor dht11;
  BMP180Sensor bmp180;
  MPL3115A2Sensor mpl3115a2;
};
}

MeasurementsTemp measResults;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Begin");
  if(!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    Serial.println("SSD1306 allocation failed");
    while(1){};
  }
  dht11.begin();
  bmp180.begin();
  mpl3115a2.begin();



  display.display();
}

void loop() 
{
  takeMeasurement();
//  printSerial();
  printOled();
  delay(20000);
}

void takeMeasurement()
{
  dht11.takeMeasurement();
  bmp180.takeMeasurement();
  mpl3115a2.takeMeasurement();

  Measurements dht11Meas = dht11.getMeasurement();
  
  measResults.dht11.temperature = dht11Meas.measurements[0].value;
  measResults.dht11.humidity = dht11Meas.measurements[1].value;

  Measurements bmp180Meas = bmp180.getMeasurement();

  measResults.bmp180.temperature = bmp180Meas.measurements[0].value;
  measResults.bmp180.pressure = bmp180Meas.measurements[1].value;

  Measurements mpl3115a2Meas = mpl3115a2.getMeasurement();
  
  measResults.mpl3115a2.pressure = mpl3115a2Meas.measurements[0].value;
  measResults.mpl3115a2.temperature = mpl3115a2Meas.measurements[1].value;  
}

//void printSerial()
//{
  //Serial.print(measResults.dht11.temperature); Serial.println("C");
  //Serial.print(measResults.dht11.humidity); Serial.println("%");
  
  //Serial.print(measResults.bmp180.temperature); Serial.println("C");
  //Serial.print(measResults.bmp180.pressure/100); Serial.println("hPa");

  //Serial.print(measResults.mpl3115a2.temperature); Serial.println("C");
  //Serial.print(measResults.mpl3115a2.pressure/100); Serial.println("hPa");
//}

void printOled()
{
  static int a=0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(measResults.dht11.temperature); display.println("C");
  display.print(measResults.dht11.humidity); display.println("%");
  display.print(measResults.bmp180.temperature); display.println("C");
  display.print(measResults.bmp180.pressure/100); display.println("hPa");
  display.print(measResults.mpl3115a2.temperature); display.println("C");
  display.print(measResults.mpl3115a2.pressure/100); display.println("hPa");
  display.println(a++);
  display.display();
}
