#ifndef IRRIGATION_HEADER_
#define IRRIGATION_HEADER_

#include <Arduino.h>
enum State { AUTO=1, MANUAL, MAINTENANCE};



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
  	void setAuto(State&);
  	void setManual(State&);
  	void setMaintenance(State&);
		void toggleMode(State&);
};

class Command
{
	public:
 		 void setWithSerial(String&,State&,bool&);
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