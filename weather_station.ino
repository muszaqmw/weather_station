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

namespace
{
struct MeasurementsTemp
{
  struct DHT11Sensor
  {
    float temperature = 0;
    String unit1;
    float humidity = 0;
    String unit2;
  };
  
  struct BMP180Sensor
  {
    float pressure = 0;
    String unit;
  };

  struct MPL3115A2Sensor
  {
    float pressure = 0;
    String unit;
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
  delay(5000);
}

void takeMeasurement()
{
  dht11.takeMeasurement();
  bmp180.takeMeasurement();
  mpl3115a2.takeMeasurement();

  Measurements * Meas = &(dht11.getMeasurement());
  Measurement * mes = Meas->measurements;
  measResults.dht11.temperature = mes->value;
  measResults.dht11.unit1 = mes->unit;
  measResults.dht11.humidity = (mes + 1)->value;
  measResults.dht11.unit2 = (mes + 1)->unit;

  Meas = &(bmp180.getMeasurement());
  mes = Meas->measurements;
  measResults.bmp180.pressure = mes->value;
  measResults.bmp180.unit = mes->unit;

  Meas = &(mpl3115a2.getMeasurement());

  mes = Meas->measurements;
  measResults.mpl3115a2.pressure = mes->value;
  measResults.mpl3115a2.unit = mes->unit;
}

void printSerial()
{
  Serial.print(measResults.dht11.temperature); Serial.println(measResults.dht11.unit1);
  Serial.print(measResults.dht11.humidity); Serial.println(measResults.dht11.unit2);

  Serial.print(measResults.bmp180.pressure/100); Serial.println(measResults.bmp180.unit);

  Serial.print(measResults.mpl3115a2.pressure/100); Serial.println(measResults.mpl3115a2.unit);
}

void printOled()
{
  static int a=0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(measResults.dht11.temperature); display.println(measResults.dht11.unit1);
  display.print(measResults.dht11.humidity); display.println(measResults.dht11.unit2);
  display.print(measResults.bmp180.pressure/100); display.println(measResults.bmp180.unit);
  display.print(measResults.mpl3115a2.pressure/100); display.println(measResults.mpl3115a2.unit);
  display.println(a++);
  display.display();
}
