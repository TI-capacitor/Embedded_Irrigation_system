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
  	int cropThreshold(Crop);
};
```
- valveOpen(): Activates signal to turn on valve
- valveClose(int); Turns off valve signal
- valveAuto(int,int,int): 
- cropThreshold(Crop): Method to set crop threshold (this is different depending on the type of crop)

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
- void setAuto(State&): Sets closed loop system with moisture sensor
- void setManual(State&): Manually choose the crops and irrigation time
- void setMaintenance(State&): turns dissengages power to all components so that the system can be worked on without powering off
- void toggleMode(State&): 
# Command class
```c++

class Command
{
	public:
 		 void setWithSerial(String&,State&,bool&,Crop&,bool&,bool&); 
};
```
setWithSerial(String&,State&,bool&,Crop&,bool&,bool&): Sets mode through serial interface (ideally through Serial Monitor App)
# Mode class
```c++
class Sensors
{
	public:
		int moistureValue(int);

};
```
moistureValue(int): 

