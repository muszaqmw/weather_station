#pragma once
#include "SensorWrapper.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

class BMP280Wrapper : public SensorWrapper
{
public:
	BMP280Wrapper()
	{
    data[0] = PRESSURE + TEMPERATURE;
	}
	
	void takeMeasurement() override
	{
    MeasurementDataPtr measData = reinterpret_cast<MeasurementDataPtr>(data + 1);
    *(measData++) = compressTemperature(bmp.readTemperature());
    *measData = compressPressure(bmp.readPressure());
	}

  void* const getData()
  {
    return static_cast<void*>(data);
  }
	
	void begin() override
	{
    Serial.println(F("BMP280 begin"));
		if(not bmp.begin(0x76))
		{
      Serial.println(F("BMP280 fail"));
			while(true){}
		}
	}

private:
  char data[5];
	Adafruit_BMP280 bmp;
};
