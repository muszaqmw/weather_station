#pragma once
#include "SensorWrapper.h"
#include "DHT.h"

namespace
{
constexpr uint8_t dhtType = 11;
}
class DHT11Adapter : public SensorAdapter
{
public:
	DHT11Adapter(const uint8_t dhtPin): dht(DHT(dhtPin, dhtType))
	{
    data[0] = TEMPERATURE + HUMIDITY;
	}
	
	void takeMeasurement() override
	{
    MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(data + 1);
    *measData = compressTemperature(dht.readTemperature());
    *(measData + 1) = static_cast<int>(dht.readHumidity());
	}

  void* const getData()
  {
    return static_cast<void*>(data);
  }
	
	void begin() override
	{
    Serial.println(F("DHT11 begin"));
		dht.begin();
	}
private:
  char data[5];
	DHT dht;
};
