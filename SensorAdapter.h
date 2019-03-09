#pragma once

class SensorAdapter
{
public:
	virtual void takeMeasurement() = 0;
	virtual void begin() = 0;
  virtual void* const getData() = 0;
	virtual ~SensorAdapter() = default;
};
