#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DS3231.h>

constexpr byte NUM_OF_SCREEN = 3;
byte screenNumber = 0;

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

volatile byte firstButtonState = 1;
volatile byte secondButtonState = 1;
volatile byte thirdButtonState = 1;
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

void incrementCounter()
{
  if(millis() - time > 200UL)
  {    
    firstButtonState = digitalRead(firstButton);
    secondButtonState = digitalRead(secondButton);
    thirdButtonState = digitalRead(thirdButton);
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
//  Clock.setSecond(10);//Set the second 
//  Clock.setMinute(20);//Set the minute 
//  Clock.setHour(18);  //Set the hour 
//  Clock.setDoW(7);    //Set the day of the week
//  Clock.setDate(19);  //Set the date of the month
//  Clock.setMonth(5);  //Set the month of the year
//  Clock.setYear(19);  //Set the year (Last two digits of the year)
  

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementCounter, FALLING);
}

void readDS3231()
{
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();  
}

void displayTime()
{ 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("20");
  display.print(year);
  display.print('-');
  display.print(month);
  display.print('-');
  display.print(date);
  display.print('\n');
  display.print(hour);
  display.print(':');
  display.print(minute);
  display.print(':');
  display.print(second);
  display.print('\n');
  display.display();
}

void changeCounter()
{
  if(not firstButtonState)
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

  if(not thirdButtonState)
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
    display.println(F("Drugi")); break;
    case 2:
    display.println(F("Trzeci")); break;

    default:
    display.println(F("DUPA")); break;
  }
  display.display();
}

void loop() 
{
  if(shouldRefreshDisplay)
  {
    changeCounter();
    refreshDisplay();
    shouldRefreshDisplay = false;
  }

  if(millis() - refreshTime > 1000UL && screenNumber == 1)
  {
    readDS3231();
    displayTime();
    refreshTime = millis();
  }
}
