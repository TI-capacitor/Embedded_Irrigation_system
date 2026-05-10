#ifndef IRRIGATION_HEADER_
#define IRRIGATION_HEADER_

#include <Arduino.h>
enum State { AUTO=1, MANUAL, MAINTENANCE};

extern State currentState;
extern bool stringComplete;
extern String inputString;


class Valve
{
  public:
  	void valveOpen(int);
		void valveClose(int);
  	void valveAuto(int,int,int);
};

class Mode
{
  public:
  	void setAuto();
  	void setManual();
  	void setMaintenance();
		void toggleMode(enum);
};

class Command
{
	public:
 		 void setWithSerial();
};

class Sensors
{
	public:
		int moistureValue(int);
		//
		//
		//

		//temperature sensor will go here in the future

};


#endif 