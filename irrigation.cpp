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
  if(reading < Threshold)
  {
  	valveOpen(v);
  }
  else
  {
    valveClose(v);
  }

}

//--------------MODE ACCESS MEMBERS ----------------/
void Mode::setAuto()
{
	currentState = AUTO;
  
}

void Mode::setManual()
{
	currentState = MANUAL;
}

void Mode::setMaintenance()
{
	currentState = MAINTENANCE;
}


void Mode::toggleMode(enum currentState){
  switch(currentState)
    {
      case AUTO:
        setManual();
        break;
      case MANUAL:
        setMaintenance();
        break;
      case MAINTENANCE:
        setAuto();
        
        break;
    }
}

//--------------COMMAND ACCESS MEMBERS ----------------/

void Command::setWithSerial()
{
  while (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c == '\n')
    {
      stringComplete = true;
    }
    else if (c != '\r')
    {
      inputString += c;
    }
  }

  if (stringComplete)
  {
    inputString.trim();
    inputString.toLowerCase();

    Serial.print("Received: ");
    Serial.println(inputString);

    if (inputString == "auto")
    {
      currentState = AUTO;
      Serial.println("Mode set to AUTO");
    }
    else if (inputString == "manual")
    {
      currentState = MANUAL;
      Serial.println("Mode set to MANUAL");
      Serial.println("Choose by typing the name of the following three options");
      Serial.println("Each crop type has a defined timer:");
      Serial.println("1. beans");
      Serial.println("2. peppers");
      Serial.println("3. tomatoes");
      
    }
    else if (inputString == "maintenance")
    {
      currentState = MAINTENANCE;
      Serial.println("Mode set to MAINTENANCE");
    }
    else
    {
      Serial.print("Unknown command: ");
      Serial.println(inputString);
    }

    inputString = "";
    stringComplete = false;
  }
}

//--------------DISPLAY ACCESS MEMBER ----------------/

//--------------SENSO ACCESS MEMBERS ----------------/

int Sensors::moistureValue(int analogPin)
{
  int read = constrain(analogRead(analogPin),0,1023);
  read = map(read,0,1023,0,255);
  return read;
}