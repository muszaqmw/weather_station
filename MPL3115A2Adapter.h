#pragma once
#include "SensorAdapter.h"
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

class MPL3115A2Adapter : public SensorAdapter
{
public:
	MPL3115A2Adapter(const Adafruit_MPL3115A2& _mpl) : mpl(_mpl), measurements(2)
	{
		measurements.measurements[0] = Measurement("Cisnienie", "hPa");
		measurements.measurements[1] = Measurement("Temperatura", "C");
	}
	
	void takeMeasurement() override
	{
		measurements.measurements[0].value = mpl.getPressure();
		measurements.measurements[1].value = mpl.getTemperature();
	}
	
	Measurements getMeasurement() override
	{
		Measurements meas = measurements;
		return meas;
	}
	
	void begin() override
	{
		if(not mpl.begin())
		{
			while(true){}
		}
	}

private:
	const Adafruit_MPL3115A2& mpl;
	Measurements measurements;
};
