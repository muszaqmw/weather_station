#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



//DHT11
constexpr uint8_t dhtDigitalPin = 2;
constexpr uint8_t dhtType = 11;

DHT temperatureHumiditySensor(dhtDigitalPin, dhtType);

//MPL3115A2
Adafruit_MPL3115A2 pressureSensor = Adafruit_MPL3115A2();

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
}

uint16_t measurementDelay = 120000;

void loop() {
  
  double temperature = temperatureHumiditySensor.readTemperature();
  if(isnan(temperature))
  {
    Serial.println("Blad w odczycie temperatury");
  }
  else
  {
    Serial.print("Aktualna temperatura: ");
    Serial.print(temperature);
    Serial.println("°C");
  }

  double humidity = temperatureHumiditySensor.readHumidity();
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
 
  Serial.print("Aktualne cisnienie: ");
  Serial.print(pressureSensor.getPressure()/100);
  Serial.println(" hPa ");

  if(true)
  {
    Serial.print("Aktualna temperatura z MPL3115A2: ");
    Serial.print(pressureSensor.getTemperature());
    Serial.println("°C\n");
  }
  
  delay(measurementDelay);
}
