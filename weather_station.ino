#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



//DHT11
constexpr uint8_t dhtDigitalPin = 2;
constexpr uint8_t dhtType = 11;

DHT temperatureHumiditySensor(dhtDigitalPin, dhtType);

//MPL3115A2
Adafruit_MPL3115A2 pressureSensor = Adafruit_MPL3115A2();

//SSD1306
constexpr uint8_t oledWidth = 128;
constexpr uint8_t oledHeight = 64;
constexpr uint8_t oledCLK = 12;
constexpr uint8_t oledMOSI = 11;
constexpr uint8_t oledReset = 10;
constexpr uint8_t oledDC = 9;
constexpr uint8_t oledCS = 8; //unusued

Adafruit_SSD1306 ssd1306Display(oledWidth, oledHeight,
  oledMOSI, oledCLK, oledDC, oledReset, oledCS);


void setup() 
{
  Serial.begin(9600);
  Serial.println("Inicjalizacja DHT11");
  temperatureHumiditySensor.begin();

  if(not pressureSensor.begin())
  {
    Serial.println("Problem z zainicjowaniem MPL3115A2");
    while(true) {};
  }

  if(not ssd1306Display.begin(SSD1306_SWITCHCAPVCC))
  {
    Serial.println("Problem z zainicjowaniem SSD1306");
    while(true) {};
  }
  ssd1306Display.display();
  delay(2000);

  ssd1306Display.clearDisplay();
}

uint32_t measurementDelay = 5000;

void loop() {
  
  float temperature = temperatureHumiditySensor.readTemperature();
  if(isnan(temperature))
  {
    Serial.println("Blad w odczycie temperatury");
  }
  else
  {
    Serial.print("Aktualna temperatura: ");
    Serial.print(temperature);
    Serial.println("Â°C");
  }

  float humidity = temperatureHumiditySensor.readHumidity();
  if(isnan(humidity))
  {
    Serial.println("Blad w odczycie wilgotnosci"); 
  }
  else
  {
    Serial.print("Aktualna wilgotnosc: ");
    Serial.print(humidity);
    Serial.println("%");   
  }

  float pressure = pressureSensor.getPressure()/100;
  Serial.print("Aktualne cisnienie: ");
  Serial.print(pressure);
  Serial.println("hPa");

  float temperature2;
  if(true)
  {
    temperature2 = pressureSensor.getTemperature();
    Serial.print("Aktualna temperatura z MPL3115A2: ");
    Serial.print(temperature2);
    Serial.println("Â°C\n");
  }

  bool hugeDisplay = true;

  if(not hugeDisplay)
  {
    ssd1306Display.clearDisplay();
    ssd1306Display.setTextSize(1);
    ssd1306Display.setTextColor(WHITE);
    ssd1306Display.setCursor(0,0);
  
    ssd1306Display.print("Temperatura: ");
    ssd1306Display.print(temperature);
    ssd1306Display.println("C");
  
    ssd1306Display.print("Wilgotnosc: ");
    ssd1306Display.print(humidity);
    ssd1306Display.println("%");
  
    ssd1306Display.print("Cisnienie: ");
    ssd1306Display.print(pressure);
    ssd1306Display.println("hPa");
  
    ssd1306Display.display();
  }
  else
  {
    ssd1306Display.clearDisplay();
    ssd1306Display.setTextSize(2);
    ssd1306Display.setTextColor(WHITE);
    ssd1306Display.setCursor(0,0);
  
    ssd1306Display.print("T:");
    ssd1306Display.print(temperature);
    ssd1306Display.println("C");
  
    ssd1306Display.print("H:");
    ssd1306Display.print(humidity);
    ssd1306Display.println("%");
  
    ssd1306Display.print("P:");
    ssd1306Display.print(pressure);
    ssd1306Display.println("hPa");
  
    ssd1306Display.display();
  }
  delay(measurementDelay);
}
