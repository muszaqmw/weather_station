#pragma once
#include "SensorAdapter.h"
#include <Adafruit_BMP085.h>

class BMP180Adapter : public SensorAdapter
{
public:
	BMP180Adapter(const Adafruit_BMP085& _bmp) : bmp(_bmp), measurements(2)
	{	
		measurements.measurements[0] = Measurement();
		measurements.measurements[1] = Measurement();
	}
	
	void takeMeasurement() override
	{
		measurements.measurements[0].value = bmp.readPressure();
		measurements.measurements[1].value = bmp.readTemperature();
	}
	
	Measurements getMeasurement() override
	{
		Measurements meas = measurements;
		return meas;
	}
	
	void begin() override
	{
    Serial.println("BMP180 begin");
		if(not bmp.begin())
		{
      Serial.println("BMP180 fail");
			while(true){}
		}
	}

private:
	const Adafruit_BMP085& bmp;
	Measurements measurements;
};
