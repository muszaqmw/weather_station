#pragma once

struct Measurement
{
  float value = 0;
  String unit;
};

struct Measurements
{
	Measurements(int _numOfMeas): numOfMeasurements(_numOfMeas)
	{}	
	Measurement * measurements;
	char numOfMeasurements;
};

class SensorAdapter
{
public:
	virtual void takeMeasurement() = 0;
	virtual Measurements getMeasurement() = 0;
	virtual void begin() = 0;
	virtual ~SensorAdapter() = default;
};
