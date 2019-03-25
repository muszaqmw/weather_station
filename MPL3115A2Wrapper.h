#pragma once
#include "SensorWrapper.h"
#include <Adafruit_MPL3115A2.h>

class MPL3115A2Adapter : public SensorAdapter
{
public:
	MPL3115A2Adapter()
	{
    data[0] = PRESSURE;
	}
	
	void takeMeasurement() override
	{
    MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(data + 1);
    *measData = compressPressure(mpl.getPressure());
	}

  void* const getData()
  {
    return static_cast<void*>(data);
  }
	
	void begin() override
	{
    Serial.println(F("MPL3115A2 begin"));
		if(not mpl.begin())
		{
      Serial.println(F("MPL3115A2 fail"));
			while(true){}
		}
	}
private:
  char data[3];
	Adafruit_MPL3115A2 mpl;
};
