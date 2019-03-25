#pragma once

enum MEASUREMENT
{
  TEMPERATURE = 1,
  HUMIDITY = 2,
  PRESSURE = 4,
};

class SensorAdapter
{
public:
	virtual void takeMeasurement() = 0;
	virtual void begin() = 0;
  virtual void* const getData() = 0;
	virtual ~SensorAdapter() = default;
};

using MeasurementTypePtr = char*;
using MeasurementDataPtr = int*;
