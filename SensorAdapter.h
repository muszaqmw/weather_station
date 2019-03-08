#pragma once

class SensorAdapter
{
public:
	virtual void takeMeasurement() = 0;
	virtual void begin() = 0;
  virtual void* const getData();
	virtual ~SensorAdapter() = default;
};
