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
    delay(3000);
  }
  else
  {
    valveClose(v);
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

void Command::setWithSerial(String &inputString, State &currentState, bool &manualMenuActive)
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
    Serial.println("Beans selected.");
    Serial.println("Starting manual watering for BEANS.");

    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "peppers")
  {
    Serial.println("Peppers selected.");
    Serial.println("Starting manual watering for PEPPERS.");

    inputString = "";
  }
  else if (manualMenuActive == true && currentState == MANUAL && inputString == "tomatoes")
  {
    Serial.println("Tomatoes selected.");
    Serial.println("Starting manual watering for TOMATOES.");

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

//--------------SENSO ACCESS MEMBERS ----------------/

int Sensors::moistureValue(int analogPin)
{
  int read = constrain(analogRead(analogPin),0,1023);
  return read;
}