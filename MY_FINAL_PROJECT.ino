#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "irrigation.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); 


unsigned long currentTime = 0;
unsigned long prevTime = 0;

const int valveSignal = 8;
const int button = 2;
const int moistureSensor = A0;

bool stringComplete = false;
bool buttonStatus = false;

int buttonCounter = 1;

String inputString = "";
Mode mode;
Command cmd;
State currentState = MAINTENANCE;
Valve valve;
Sensors sensor;





void setup() 
{
  Serial.begin(9600);
  pinMode(valveSignal,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), buttonInterrupt, FALLING);
  pinMode(moistureSensor,INPUT);
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


  cmd.setWithSerial();
  lcdDisplay(currentState, moisture);


  if(buttonStatus)
  {
    buttonCounter++;
    if(buttonCounter == 1)
      mode.setAuto();
    if(buttonCounter == 2)
      mode.setManual();
    if(buttonCounter == 3)
    {
      mode.setMaintenance();
  
    
    //do something here
    switch(currentState)
    {
      case AUTO:
        valve.valveAuto(moisture, 130, valveSignal);
        break;
      case MANUAL:
        mode.setManual();
        break;
      case MAINTENANCE:
        mode.setMaintenance();
        break;
    }
    
    buttonStatus = false;
  }

  

}

void buttonInterrupt()
{
  if(button == LOW)
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
      lcd.print("     ");
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
        Serial.println("MANUAL");
     }
     else if(currentState == MAINTENANCE)
     {
        lcd.print("MAINTENANCE");
        Serial.println("MAINTENANCE");
     }
}

