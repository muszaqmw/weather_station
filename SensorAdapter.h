#pragma once

struct Measurement
{
	Measurement() = default;
	Measurement(String _descr, String _unit):
		description(_descr),
		unit(_unit)
	{}		
	
	String getValueWithDescription()
	{
		return String(description + " " +  value + unit);
	}
	
	String description = "";
	float value = 0;
	String unit = "";
};

struct Measurements
{
	Measurements(int _numOfMeas): numOfMeasurements(_numOfMeas)
	{
		measurements = new Measurement[numOfMeasurements];
	}
	
	~Measurements()
	{
		delete[] measurements;
	}
	
	Measurement * measurements;
	int numOfMeasurements;
};

class SensorAdapter
{
public:
	virtual void takeMeasurement() = 0;
	virtual Measurements getMeasurement() = 0;
	virtual void begin() = 0;
	virtual ~SensorAdapter() = default;
};
