#pragma once
#include "SensorAdapter.h"
#include "DHT.h"

class DHT11Adapter : public SensorAdapter
{
public:
	DHT11Adapter(const DHT& _dht): dht(_dht), measurements(2)
	{	
		measurements.measurements[0] = Measurement();
		measurements.measurements[1] = Measurement();
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
    Serial.println("DHT11 begin");
		dht.begin();
	}
	
private:
	const DHT& dht;
	Measurements measurements;
};
