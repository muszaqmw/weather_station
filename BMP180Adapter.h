#pragma once
#include "SensorAdapter.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

class BMP180Adapter : public SensorAdapter
{
public:
	BMP180Adapter(const Adafruit_BMP085& _bmp) : bmp(_bmp), measurements(2)
	{	
		bmp = Adafruit_BMP085();
		
		measurements.measurements[0] = Measurement("Cisnienie", "hPa");
		measurements.measurements[1] = Measurement("Temperatura", "C");
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
		if(not bmp.begin())
		{
			while(true){}
		}
	}

private:
	const Adafruit_BMP085& bmp;
	Measurements measurements;
};
