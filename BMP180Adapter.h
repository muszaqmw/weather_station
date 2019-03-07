#pragma once
#include "SensorAdapter.h"
#include <Adafruit_BMP085.h>

class BMP180Adapter : public SensorAdapter
{
public:
	BMP180Adapter() : measurements(2)
	{}

  ~BMP180Adapter()
  {
    delete[] measurements.measurements;
  }
	
	void takeMeasurement() override
	{
		measurements.measurements[0].value = bmp.readTemperature();
		measurements.measurements[1].value = bmp.readPressure();
	}
	
	Measurements getMeasurement() override
	{
		Measurements meas = measurements;
		return meas;
	}
	
	void begin() override
	{
    Serial.println(F("BMP180 begin"));
		if(not bmp.begin())
		{
      Serial.println(F("BMP180 fail"));
			while(true){}
		}
    measurements.measurements = new Measurement[2];
	}

private:
	Adafruit_BMP085 bmp;
	Measurements measurements;
};
