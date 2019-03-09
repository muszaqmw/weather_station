#pragma once
#include "SensorAdapter.h"
#include <Adafruit_MPL3115A2.h>

class MPL3115A2Adapter : public SensorAdapter
{
public:
	MPL3115A2Adapter()
	{
    data[0] = 0b00000100;
	}
	
	void takeMeasurement() override
	{
    int* measData = reinterpret_cast<int*>(data + 1);
    *measData = static_cast<int>(mpl.getPressure()/10);
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
