#pragma once

enum MEASUREMENT
{
  TEMPERATURE = 1,
  HUMIDITY = 2,
  PRESSURE = 4,
};

class SensorWrapper
{
public:
	virtual void takeMeasurement() = 0;
	virtual void begin() = 0;
  virtual void* const getData() = 0;
	virtual ~SensorWrapper() = default;
};

using MeasurementTypePtr = char*;
using MeasurementDataPtr = int*;

int compressTemperature(float rawTemperature)
{
  return static_cast<int>(rawTemperature*100);
}

float decompressTemperature(int temperature)
{
  return temperature/100.;
}

int compressPressure(float rawPressure)
{
  return static_cast<int>(rawPressure/10);
}

float decompressPressure(int pressure)
{
  return pressure/10.;
}
