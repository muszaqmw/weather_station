#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DS3231.h>

constexpr byte NUM_OF_SCREEN = 3;
byte screenNumber = 1;
byte currentSettingValue = 0;

constexpr byte interruptPin = 2;
constexpr byte firstButton = 7;
constexpr byte secondButton = 6;
constexpr byte thirdButton = 5;

constexpr byte SCREEN_WIDTH = 128;
constexpr byte SCREEN_HEIGHT = 64;

constexpr byte OLED_MOSI = 11;
constexpr byte OLED_CLK  = 12;
constexpr byte OLED_DC   = 9;
constexpr byte OLED_CS   = 8;
constexpr byte OLED_RESET = 10;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

volatile bool isFirstButtonPushed = 0;
volatile bool isSecondButtonPushed = 0;
volatile bool isThirdButtonPushed = 0;
bool isSettingMode = false;
unsigned long time = 0;

bool shouldRefreshDisplay = true;


//new
DS3231 Clock;
bool Century = false;
bool PM;
bool h12;

byte year, month, date, DoW, hour, minute, second;
unsigned long refreshTime = 0;
//end_new

void pushedButton()
{
  if(millis() - time > 200UL)
  {    
    isFirstButtonPushed = not digitalRead(firstButton);
    isSecondButtonPushed = not digitalRead(secondButton);
    isThirdButtonPushed = not digitalRead(thirdButton);
    shouldRefreshDisplay = true;
    time = millis();
  }
}

void setup() 
{
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    while(true);
  }
  Wire.begin();
//  Clock.setSecond(0);//Set the second 
//  Clock.setMinute(20);//Set the minute 
//  Clock.setHour(19);  //Set the hour 
//  Clock.setDoW(7);    //Set the day of the week
//  Clock.setDate(26);  //Set the date of the month
//  Clock.setMonth(5);  //Set the month of the year
//  Clock.setYear(19);  //Set the year (Last two digits of the year)

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), pushedButton, FALLING);
}

void getTime()
{
  second = Clock.getSecond();//Set the second 
  minute = Clock.getMinute();//Set the minute 
  hour = Clock.getHour(h12, PM);  //Set the hour 
  DoW = Clock.getDoW();    //Set the day of the week
  date = Clock.getDate();  //Set the date of the month
  month = Clock.getMonth(Century);  //Set the month of the year
  year = Clock.getYear();  //Set the year (Last two digits of the year)
}

void setClock()
{
  Clock.setSecond(second);//Set the second 
  Clock.setMinute(minute);//Set the minute 
  Clock.setHour(hour);  //Set the hour 
  Clock.setDoW(DoW);    //Set the day of the week
  Clock.setDate(date);  //Set the date of the month
  Clock.setMonth(month);  //Set the month of the year
  Clock.setYear(year);  //Set the year (Last two digits of the year)
}

void displayTime()
{
  display.print("20");display.print(year);display.print(F("-"));display.print(month); display.print(F("-"));display.println(date);
  display.print(hour);display.print(F(":"));display.print(minute);display.print(F(":"));display.println(second);
  if(isSettingMode)
  {
    display.print(currentSettingValue);
  }
}

void changeValue(byte& value)
{
  if(isFirstButtonPushed)
  {
    --value;
  }
  if(isThirdButtonPushed)
  {
    ++value;
  }
}

void handleButton()
{
  if(isSettingMode == false)
  {
    if(isFirstButtonPushed)
    {
      if(screenNumber == 0)
      {
        screenNumber = NUM_OF_SCREEN - 1;
      }
      else
      {
        --screenNumber;
      }
    }
    if(isThirdButtonPushed)
    {
      if(screenNumber == NUM_OF_SCREEN - 1)
      {
        screenNumber = 0;
      }
      else
      {
        ++screenNumber;
      }
    }
    if(isSecondButtonPushed && screenNumber == 1)
    {
      isSettingMode = true;
    }
  }
  else
  {
    if(isSecondButtonPushed)
    {
      if(currentSettingValue == 6)
      {
        currentSettingValue = 0;
        isSettingMode = false;
      }
      else
      {
        setClock();
        ++currentSettingValue;
      }
      
    }
    switch(currentSettingValue)
    {
      case 0:
      changeValue(year); break;
      case 1:
      changeValue(month); break;
      case 2:
      changeValue(date); break;
      case 3:
      changeValue(hour); break;
      case 4:
      changeValue(minute); break;
      case 5:
      changeValue(second); break;
    }
  }
}


void refreshDisplay()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  switch (screenNumber)
  {
    case 0: 
    display.println(F("Pierwszy")); break;
    case 1:
    displayTime(); break;
    case 2:
    display.println(F("Trzeci")); break;
  }
  display.display();
}

void loop() 
{
  if(shouldRefreshDisplay)
  {
    handleButton();
    refreshDisplay();
    shouldRefreshDisplay = false;
  }

  if(millis() - refreshTime > 1000UL && screenNumber == 1 && not isSettingMode)
  {
    getTime();
    refreshDisplay();
    refreshTime = millis();
  }
}
