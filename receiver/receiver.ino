#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr uint8_t NUM_OF_SCREEN = 3;
volatile uint8_t screenNumber = 0;

constexpr uint8_t interruptPin = 2;
constexpr uint8_t firstButton = 7;
constexpr uint8_t secondButton = 6;
constexpr uint8_t thirdButton = 5;

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr uint8_t OLED_MOSI = 11;
constexpr uint8_t OLED_CLK  = 12;
constexpr uint8_t OLED_DC   = 9;
constexpr uint8_t OLED_CS   = 8;
constexpr uint8_t OLED_RESET = 10;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

volatile uint8_t firstButtonState = 1;
volatile uint8_t secondButtonState = 1;
volatile uint8_t thirdButtonState = 1;
unsigned long time = 0;

bool shouldRefreshDisplay = true;


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

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(firstButton, INPUT_PULLUP);
  pinMode(secondButton, INPUT_PULLUP);
  pinMode(thirdButton, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementCounter, FALLING);
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
  
}
