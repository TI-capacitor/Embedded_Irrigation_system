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

int buttonCounter = 1;

String inputString = "";
Mode mode;
Command cmd;
State currentState = MAINTENANCE;
Valve valve;
Sensors sensor;



void buttonInterrupt()
{
  buttonStatus = true;
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
      Serial.println(moisture);
    }
  
     else if(currentState == MANUAL)
     {
      	lcd.setCursor(0,1);
   	  	lcd.print("            ");
      	lcd.setCursor(5,0);
        lcd.print("MANUAL");
     }
     else if (currentState == MAINTENANCE)
    {
      lcd.print("Maintenance");

      lcd.setCursor(0, 1);
      lcd.print("Valve OFF     ");

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
  delay(200);
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
  cmd.setWithSerial(inputString,currentState,manualActive);

  if(buttonStatus)
  {
    mode.toggleMode(currentState);
    buttonStatus = false;
  }

  lcdDisplay(currentState, moisture);
  
  //if(prevTime - currentTime)
    if(currentState == AUTO)
    {
      valve.valveAuto(moisture, 900, valveSignal);
      
    }
    if(buttonCounter == MANUAL)
    {
      //probably not needed
    }
    if(currentState == AUTO)
    {
      valve.valveClose(valveSignal);
    }

 }
    




  






