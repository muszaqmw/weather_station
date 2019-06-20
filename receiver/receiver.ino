#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <DS3232RTC.h>

constexpr byte NUM_OF_SCREEN = 3;
byte screenNumber = 1;
byte currentSettingValue = 0;

constexpr byte buttonInterruptPin = 2;
constexpr byte rtcInterruptPin = 3;
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
uint16_t YEAR;
byte MONTH, DAY, HOUR, MINUTE, SECOND;


//new
bool Century = false;
bool PM;
bool h12;

time_t rtcTime;
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

void wakeUp()
{
  sleep_disable();//Disable sleep mode
  detachInterrupt(digitalPinToInterrupt(rtcInterruptPin));
}

//time_t compileTime()
//{
//    const time_t FUDGE(10);    //fudge factor to allow for upload time, etc. (seconds, YMMV)
//    const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
//    char compMon[3], *m;
//
//    strncpy(compMon, compDate, 3);
//    compMon[3] = '\0';
//    m = strstr(months, compMon);
//
//    tmElements_t tm;
//    tm.Month = ((m - months) / 3 + 1);
//    tm.Day = atoi(compDate + 4);
//    tm.Year = atoi(compDate + 7) - 1970;
//    tm.Hour = atoi(compTime);
//    tm.Minute = atoi(compTime + 3);
//    tm.Second = atoi(compTime + 6);
//
//    time_t t = makeTime(tm);
//    return t + FUDGE;        //add fudge factor to allow for compile time
//}

void setup() 
{
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) 
  {
    while(true);
  }
//  Clock.setSecond(0);//Set the second 
//  Clock.setMinute(20);//Set the minute 
//  Clock.setHour(19);  //Set the hour 
//  Clock.setDoW(7);    //Set the day of the week
//  Clock.setDate(26);  //Set the date of the month
//  Clock.setMonth(5);  //Set the month of the year
//  Clock.setYear(19);  //Set the year (Last two digits of the year)

  pinMode(buttonInterruptPin, INPUT_PULLUP);
  pinMode(rtcInterruptPin, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //RTC.set(compileTime());

  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);


  RTC.setAlarm(ALM1_EVERY_SECOND, 0, 0, 0, 0);// Setting alarm 1
  RTC.alarm(ALARM_1);
  // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
  RTC.squareWave(SQWAVE_NONE);
  // enable interrupt output for Alarm 2
  RTC.alarmInterrupt(ALARM_1, true);
  
  attachInterrupt(digitalPinToInterrupt(buttonInterruptPin), pushedButton, FALLING);
}

void getTime()
{
  time = RTC.get();
  YEAR = year(time);
  MONTH = month(time);
  DAY = day(time);
  HOUR = hour(time);
  MINUTE = minute(time);
  SECOND = second(time);  
}

String alignDigit(const byte & number)
{
  String textValue = String(number);
  
  if(number < 10)
  {
     return String("0" + textValue);
  }
  return textValue;
}

void displayTime()
{
  display.print(YEAR);display.print(F("-"));display.print(alignDigit(MONTH)); display.print(F("-"));display.println(alignDigit(DAY));
  display.print(alignDigit(HOUR));display.print(F(":"));display.print(alignDigit(MINUTE));display.print(F(":"));display.println(alignDigit(SECOND));
  if(isSettingMode)
  {
    display.print(currentSettingValue);
  }
}

template <typename T>
void changeValue(T& value)
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

void setClock()
{
  tmElements_t newTime;

  if (YEAR >= 1000)
  {
    newTime.Year = CalendarYrToTm(YEAR);
  }
  else
  {
    newTime.Year = y2kYearToTm(YEAR);
  }
  newTime.Year = YEAR - 1970;
  newTime.Month = MONTH;
  newTime.Day = DAY;
  newTime.Hour = HOUR;
  newTime.Minute = MINUTE;
  newTime.Second = SECOND;

  RTC.set(makeTime(newTime));
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
      changeValue(YEAR); break;
      case 1:
      changeValue(MONTH); break;
      case 2:
      changeValue(DAY); break;
      case 3:
      changeValue(HOUR); break;
      case 4:
      changeValue(MINUTE); break;
      case 5:
      changeValue(SECOND); break;
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

void goingToSleep()
{
    sleep_enable();//Enabling sleep mode
    attachInterrupt(digitalPinToInterrupt(rtcInterruptPin), wakeUp, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN, LOW);//turning LED off
   
    delay(100); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
    
    digitalWrite(LED_BUILTIN, HIGH);//turning LED on
}

void loop() 
{
  if(RTC.alarm(ALARM_1) && screenNumber == 1 && not isSettingMode)
  {
    getTime();
    refreshDisplay();
    refreshTime = millis();
    goingToSleep();
  }
  else if(shouldRefreshDisplay)
  {
    handleButton();
    refreshDisplay();
    shouldRefreshDisplay = false;
  }
}
