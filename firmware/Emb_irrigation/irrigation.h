#ifndef IRRIGATION_HEADER_
#define IRRIGATION_HEADER_

#include <Arduino.h>
enum State { AUTO=1, MANUAL, MAINTENANCE};

//enumerated list for state machine
enum Crop { NONE_CROP=0, BEANS, PEPPERS, TOMATOES };



class Valve
{
  public:
  	void valveOpen(int);
	void valveClose(int);
  	void valveAuto(int,int,int);
  	int cropThreshold(Crop); //NEW: returns the moisture threshold for a given crop, used by valveAuto() instead of a hardcoded number
};

class Mode
{
  public:
  	void setAuto(State&);
  	void setManual(State&);
  	void setMaintenance(State&);
	void toggleMode(State&);
};

class Command
{
	public:
 		 void setWithSerial(String&,State&,bool&,Crop&,bool&,bool&); 
};

class Sensors
{
	public:
		int moistureValue(int);

};

#endif