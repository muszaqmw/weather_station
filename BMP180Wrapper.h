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
    MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(data + 1);
    *measData = compressPressure(bmp.readPressure());
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
