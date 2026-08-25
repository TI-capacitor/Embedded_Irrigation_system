## Embedded Irrigation System

# Installation
On linux:
```bash
git clone git@github.com:TI-capacitor/Embedded_Irrigation_system.git
```
Then change into the directory:
```bash
cd /Embedded_Irrigation_system.git
```
## Overview of Irrigation.h 

# Valve class
```c++
class Valve
{
  public:
  	void valveOpen(int);
	void valveClose(int);
  	void valveAuto(int,int,int);
  	int cropThreshold(Crop); //NEW: returns the moisture threshold for a given crop, used by valveAuto() instead of a hardcoded number
};
```
- valveOpen()
- valveClose(int);
- valveAuto(int,int,int);
- cropThreshold(Crop);

#Mode class
```c++
class Mode
{
  public:
  	void setAuto(State&);
  	void setManual(State&);
  	void setMaintenance(State&);
	void toggleMode(State&);
};
```
# Command class
```c++

class Command
{
	public:
 		 void setWithSerial(String&,State&,bool&,Crop&,bool&,bool&); 
};
```
# Mode class
```c++
class Sensors
{
	public:
		int moistureValue(int);

};
```
```
