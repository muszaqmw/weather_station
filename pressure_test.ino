#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

void printSerial();
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


Adafruit_BMP085 bmp;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

DHT dht(DHTPIN, DHTTYPE);


struct Measurements
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

  DHT11Sensor       dht11;
  BMP180Sensor      bmp180;
  MPL3115A2Sensor   mpl3115a2;
};

Measurements measResults;

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  
  if (!bmp.begin())
  {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  if (!baro.begin()) 
  {
    Serial.println("Couldnt find sensor");
    while(1){}
    return;
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    Serial.println("SSD1306 allocation failed");
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
  measResults.dht11.temperature = dht.readTemperature();
  measResults.dht11.humidity = dht.readHumidity();

  measResults.bmp180.temperature = bmp.readTemperature();
  measResults.bmp180.pressure = bmp.readPressure();

  measResults.mpl3115a2.pressure = baro.getPressure();
  measResults.mpl3115a2.temperature = baro.getTemperature();  
}

void printSerial()
{
  Serial.print(measResults.dht11.temperature); Serial.println("C");
  Serial.print(measResults.dht11.humidity); Serial.println("%");
  
  Serial.print(measResults.bmp180.temperature); Serial.println("C");
  Serial.print(measResults.bmp180.pressure/100); Serial.println("hPa");

  Serial.print(measResults.mpl3115a2.temperature); Serial.println("C");
  Serial.print(measResults.mpl3115a2.pressure/100); Serial.println("hPa");
}

void printOled()
{
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
  display.display();
}
