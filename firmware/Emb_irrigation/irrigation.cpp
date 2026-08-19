#include "irrigation.h"
//--------------VALVE ACCESS MEMBERS ----------------/
void Valve::valveOpen(int v)
{
  digitalWrite(v,HIGH);
}

void Valve::valveClose(int v)
{
  digitalWrite(v,LOW);
}


void Valve::valveAuto(int reading,int Threshold,int v)
{
  
  if(reading > Threshold)
  {
  	valveOpen(v);
  }
  else
  {
    valveClose(v);
  }

}

int Valve::cropThreshold(Crop crop)
{
  switch(crop)
  {
    case BEANS:
      return 850;
    case PEPPERS:
      return 800;
    case TOMATOES:
      return 900;
    default:
      return 900;
  }
}

//--------------MODE ACCESS MEMBERS ----------------/
void Mode::setAuto(State &currentState)
{

	currentState = AUTO;

}

void Mode::setManual(State &currentState)
{
	currentState = MANUAL;
}

void Mode::setMaintenance(State &currentState)
{
	currentState = MAINTENANCE;
}


void Mode::toggleMode(State &currentState){
  switch(currentState)
    {
      case AUTO:
        setManual(currentState);
        break;
      case MANUAL:
        setMaintenance(currentState);
        break;
      case MAINTENANCE:
        setAuto(currentState);
        break;
    }
}

//--------------COMMAND ACCESS MEMBERS ----------------/

void Command::setWithSerial(String &inputString, State &currentState, bool &manualMenuActive, Crop &currentCrop, bool &waterNow, bool &stopWater)
{
  bool stringComplete = false;

  while (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c == '\n' || c == '\r')
    {
      stringComplete = true;
    }
    else
    {
      inputString += c;
    }
  }

  inputString.trim();
  inputString.toLowerCase();

  if (inputString == "auto")
  {
    currentState = AUTO;
    manualMenuActive = false;

    Serial.println("Mode set to AUTO");
    inputString = "";
  }
  else if (inputString == "manual")
  {
    currentState = MANUAL;
    manualMenuActive = true;

    Serial.println("Mode set to MANUAL");
    Serial.println("Choose by typing:");
    Serial.println("beans");
    Serial.println("peppers");
    Serial.println("tomatoes");

    inputString = "";
  }
  else if (inputString == "maintenance")
  {
    currentState = MAINTENANCE;
    manualMenuActive = false;

    Serial.println("Mode set to MAINTENANCE");
    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "beans")
  {
    currentCrop = BEANS; 
    Serial.println("Beans selected.");
    Serial.println("Type 'water' to start manual watering, or switch to AUTO."); 

    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "peppers")
  {
    currentCrop = PEPPERS; //NEW
    Serial.println("Peppers selected.");
    Serial.println("Type 'water' to start manual watering, or switch to AUTO."); //NEW

    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "tomatoes")
  {
    currentCrop = TOMATOES; //NEW
    Serial.println("Tomatoes selected.");
    Serial.println("Type 'water' to start manual watering, or switch to AUTO."); //NEW

    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "water")
 
  {
    if (currentCrop == NONE_CROP)
    {
      Serial.println("No crop selected yet. Choose beans, peppers, or tomatoes first.");
    }
    else
    {
      waterNow = true; //NEW: one-shot flag - MY_FINAL_PROJECT.ino sees this, starts the timed watering pulse, and clears it back to false
      Serial.println("Manual watering started.");
    }

    inputString = "";
  }
  else if (currentState == MANUAL && inputString == "stop")
  
  {
    stopWater = true; //NEW: one-shot flag - MY_FINAL_PROJECT.ino sees this, closes the valve, and clears it back to false
    Serial.println("Manual watering stopped.");

    inputString = "";
  }
  else if (stringComplete && inputString.length() > 0)
  {
    Serial.print("Unknown command: ");
    Serial.println(inputString);

    if (currentState == MANUAL && manualMenuActive == true)
    {
      Serial.println("Still in MANUAL mode.");
      Serial.println("Choose: beans, peppers, or tomatoes");
    }

    inputString = "";
  }
}

//--------------SENSOR ACCESS MEMBERS ----------------/

int Sensors::moistureValue(int analogPin)
{
  int read = constrain(analogRead(analogPin),0,1023);
  return read;
}