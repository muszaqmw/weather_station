#pragma once
#include "SensorAdapter.h"
#include <Adafruit_BMP085.h>

class BMP180Adapter : public SensorAdapter
{
public:
	BMP180Adapter()
	{
    data[0] = 0b00000100;
	}
	
	void takeMeasurement() override
	{
    int* measData = reinterpret_cast<int*>(data + 1);
    *measData = static_cast<int>(bmp.readPressure()/10);
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
  char data[3];
private:
	Adafruit_BMP085 bmp;
};
