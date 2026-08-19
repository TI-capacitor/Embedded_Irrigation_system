#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "irrigation.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long currentTime = 0;
unsigned long prevTime = 0;

const int valveSignal = 8;
const int button = 2;
const int moistureSensor = A0;

volatile bool buttonStatus = false;
bool manualActive = false; //manual mode menu flag

String inputString = "";
Mode mode;
Command cmd;
State currentState = MAINTENANCE;
Valve valve;
Sensors sensor;


Crop currentCrop = NONE_CROP;

//boolean flags in case user types water or stop
bool waterNow = false;
bool stopWater = false;

//non blocking 
bool manualWatering = false;
unsigned long manualWaterStart = 0;
const unsigned long MANUAL_PULSE_MS = 5000; //NEW: how long a single "water" command runs the valve for - adjust to taste



void buttonInterrupt()
{
  buttonStatus = true;
}

//NEW: small helper just for turning a Crop value into readable text for the LCD/Bluetooth
//output. Not a class since it doesn't hold any state or belong to one particular module -
//it's the same kind of plain helper style as buttonInterrupt() above.
String cropName(Crop c)
{
  switch(c)
  {
    case BEANS:   
     return "BEANS";

    case PEPPERS:  
     return "PEPPERS";

    case TOMATOES: 
      return "TOMATOES";

    default:       
      return "NONE";
  }
}

void lcdDisplay(State,int moisture)
{
lcd.setCursor(0,0);
    lcd.print("MODE:");
    lcd.print("              ");
  	lcd.setCursor(5,0);
	if(currentState == AUTO)
    {
      lcd.print("AUTO");
      lcd.setCursor(0,1);
      lcd.print("          ");
      lcd.setCursor(0,1);
      lcd.print("H:");
      lcd.print(moisture);
      lcd.print(" C:");
      lcd.print(cropName(currentCrop)[0]); //NEW: show the first letter of the selected crop (B/P/T/N) - the 16x2 LCD doesn't have room for the full word next to the moisture reading

      //NEW: telemetry now goes out every AUTO loop, same as before, just with mode+crop added
      //so the Bluetooth app has more context than a bare number.
      Serial.print("AUTO,");
      Serial.print(cropName(currentCrop));
      Serial.print(",moisture=");
      Serial.println(moisture);
    }

     else if(currentState == MANUAL)
     {
      	lcd.setCursor(0,1);
   	  	lcd.print("            ");
      	lcd.setCursor(5,0);
        lcd.print("MANUAL");

        //NEW: MANUAL used to show nothing on line 2 at all. Now it shows the selected crop
        //and whether a manual watering pulse is currently running, matching what AUTO shows.
        lcd.setCursor(0,1);
        if(manualWatering)
        {
          lcd.print("WATERING...   ");
        }
        else
        {
          lcd.print("Crop:");
          lcd.print(cropName(currentCrop));
          lcd.print("   ");
        }

        //NEW: MANUAL previously sent nothing over Serial - this fills the gap so the Bluetooth
        //app/data log stays current no matter which mode you're in (Technical Concept #9).
        Serial.print("MANUAL,");
        Serial.print(cropName(currentCrop));
        Serial.print(",moisture=");
        Serial.print(moisture);
        Serial.print(",watering=");
        Serial.println(manualWatering ? "yes" : "no");
     }
     else if (currentState == MAINTENANCE)
    {
      lcd.print("Maintenance");

      lcd.setCursor(0, 1);
      lcd.print("Valve OFF     ");

      valve.valveClose(valveSignal); //NEW: MAINTENANCE printed "Valve OFF" before but never actually guaranteed the valve was closed - this makes sure it really is, every loop while in this mode

      Serial.println("MAINTENANCE");
    }
}


void setup()
{
  Serial.begin(9600);
  pinMode(valveSignal,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), buttonInterrupt, FALLING);
  pinMode(moistureSensor,INPUT);
  lcd.clear();
  delay(200); //delay for LCD startup
  lcd.backlight();
  lcd.begin(16,2);
  lcd.print("AUTO IRRIGATION");
  lcd.setCursor(0,1);
  lcd.print("    SYSTEM    ");
  // put your setup code here, to run once:

}

void loop()
 {
  currentTime = millis();
  int moisture = sensor.moistureValue(moistureSensor);
  cmd.setWithSerial(inputString,currentState,manualActive,currentCrop,waterNow,stopWater);

  if(buttonStatus)
  {
    mode.toggleMode(currentState);
    buttonStatus = false;
  }

  lcdDisplay(currentState, moisture);

  if(prevTime - currentTime) {
    if(currentState == AUTO)
    {
      valve.valveAuto(moisture, valve.cropThreshold(currentCrop), valveSignal);
    }
    else
    {

      valve.valveClose(valveSignal);
    }

    //NEW: manual watering handling. Mirrors the non-blocking pattern used for AUTO: no delay(),
    //just checking millis() against a stored start time each loop.
    if(currentState == MANUAL)
    {
      if(waterNow && !manualWatering)
      {
        valve.valveOpen(valveSignal);
        manualWatering = true;
        manualWaterStart = currentTime;
        waterNow = false; //consume the one-shot request from Command
      }

      if(stopWater && manualWatering)
      {
        valve.valveClose(valveSignal);
        manualWatering = false;
        stopWater = false; //consume the one-shot request from Command
      }

      if(manualWatering && (currentTime - manualWaterStart >= MANUAL_PULSE_MS))
      {
        valve.valveClose(valveSignal);
        manualWatering = false;
      }
    }
    else if(manualWatering)
    {
      //NEW: safety net - if the mode changes away from MANUAL mid-pulse (button interrupt,
      //Bluetooth "auto"/"maintenance" command, etc.), don't leave the valve running unattended
      valve.valveClose(valveSignal);
      manualWatering = false;
    }
  }
 }