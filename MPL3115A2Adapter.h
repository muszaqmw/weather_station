#pragma once
#include "SensorAdapter.h"
#include <Adafruit_MPL3115A2.h>

class MPL3115A2Adapter : public SensorAdapter
{
public:
	MPL3115A2Adapter() : measurements(1)
	{}

  ~MPL3115A2Adapter()
  {
    delete[] measurements.measurements;
  }
	
	void takeMeasurement() override
	{
		measurements.measurements[0].value = mpl.getPressure();
	}
	
	Measurements getMeasurement() override
	{
		Measurements meas = measurements;
		return meas;
	}
	
	void begin() override
	{
    Serial.println(F("MPL3115A2 begin"));
		if(not mpl.begin())
		{
      Serial.println(F("MPL3115A2 fail"));
			while(true){}
		}
    measurements.measurements = new Measurement[measurements.numOfMeasurements];
    measurements.measurements[0].unit = F("hPa");
	}

private:
	Adafruit_MPL3115A2 mpl;
	Measurements measurements;
};
