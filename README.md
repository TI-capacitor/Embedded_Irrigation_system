## Embedded Irrigation System

# Installation
On linux:
```bash
git clone git@github.com:TI-capacitor/Embedded_Irrigation_system.git
```
# Overview of Irrigation.h 
Description of Irrigation.h methods

```c++
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
```
