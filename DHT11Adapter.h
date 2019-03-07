#pragma once
#include "SensorAdapter.h"
#include "DHT.h"

constexpr uint8_t dhtType = 11;

class DHT11Adapter : public SensorAdapter
{
public:
	DHT11Adapter(const uint8_t dhtPin): dht(DHT(dhtPin, dhtType)), measurements(2)
	{}

  ~DHT11Adapter()
  {
    delete[] measurements.measurements;
  }
	
	void takeMeasurement() override
	{
		measurements.measurements[0].value = dht.readTemperature();
		measurements.measurements[1].value = dht.readHumidity();
	}
	
	Measurements getMeasurement() override
	{
		Measurements meas = measurements;
		return meas;
	}
	
	void begin() override
	{
    Serial.println(F("DHT11 begin"));
		dht.begin();
    measurements.measurements = new Measurement[2];
	}
	
private:
	DHT dht;
	Measurements measurements;
};
