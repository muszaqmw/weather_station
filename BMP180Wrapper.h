#pragma once
#include "SensorWrapper.h"
#include <Adafruit_BMP085.h>

class BMP180Adapter : public SensorAdapter
{
public:
	BMP180Adapter()
	{
    data[0] = PRESSURE;
	}
	
	void takeMeasurement() override
	{
    int* measData = reinterpret_cast<int*>(data + 1);
    *measData = static_cast<int>(bmp.readPressure()/10);
	}

  void* const getData()
  {
    return static_cast<void*>(data);
  }
	
	void begin() override
	{
    Serial.println(F("BMP180 begin"));
		if(not bmp.begin())
		{
      Serial.println(F("BMP180 fail"));
			while(true){}
		}
	}

private:
  char data[3];
	Adafruit_BMP085 bmp;
};
