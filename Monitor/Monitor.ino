#include <LiquidCrystal.h>

/* LCD */
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

long screenPreviousTiming = 0.0;
long screenInterval = 2000;

int screenIndex = 0;
int screenLastIndex = 4;

boolean screenChanged = true;

#define SCREEN_TEMPERATURE  0
#define SCREEN_LEVEL        1
#define SCREEN_PUMP         2
#define SCREEN_HEATER       3
#define SCREEN_COOLER       4

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/* Temperature */
#define TEMP 0

float Vin=5.0;     // [V]        
float Rt=10000;    // Resistor t [ohm]
float R0=10000;    // value of rct in T0 [ohm]
float T0=298.15;   // use T0 in Kelvin [K]
float Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
float T1=273.15;   // [K] in datasheet 0º C
float T2=373.15;   // [K] in datasheet 100° C
float RT1=35563;   // [ohms]  resistence in T1
float RT2=549;     // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output

/* Water Level */
#define TRIG 13
#define ECHO 10
#define HEIGHT 37

/* Misc */
#define PUMP 9
#define HEATER 8
#define COOLER 7

boolean pumpIsOn = false;
boolean heaterIsOn = false;
boolean coolerIsOn = false;

boolean isError = false;

void setup()
{
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.print("Initializing...");

  beta = (log(RT1 / RT2)) / ((1 / T1) - (1 / T2));
  Rinf = R0 * exp(-beta / T0);

  // Setting the default values...
  digitalWrite(PUMP, HIGH);
  digitalWrite(HEATER, LOW);
  digitalWrite(COOLER, LOW);

  Serial.begin(9600);
  delay(2000);
  
  do
  {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Unable to");
    
    lcd.setCursor(0, 1);
    lcd.print("connect to PC.");
    
    checkSerialConn();

    if (isError)
    {
      delay(1000);
    }
  }
  while (isError);

  lcd.clear();
  lcd.setCursor(0, 0);
  
  lcd.print("Connected!");

  /* Setting up the pins for temperature. */
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  /* Setting up the pins for misc. */
  pinMode(PUMP, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(COOLER, OUTPUT);
}

void checkSerialConn()
{
  Serial.print("i");
  
  if (await() != 'o')
  {
    isError = true;
  }

  clearPort();
}

char await()
{
  while (Serial.available() < 1);
  return Serial.read();
}

void clearPort()
{
  while (Serial.available() > 0)
  {
    Serial.read();
  }
}

void loop()
{
  float temp = getTemperature();
  Serial.println(temp);
  
  long level = getWaterLevel();
  Serial.println(level);

  delay(20);

  while (Serial.available() >= 1)
  {
    processSerialInput();
  }

  unsigned long screenCurrentTiming = millis();

  if (screenCurrentTiming - screenPreviousTiming > screenInterval)
  {
    screenPreviousTiming = screenCurrentTiming;
    screenChanged = true;
    
    if (++screenIndex > screenLastIndex)
    {
      screenIndex = 0;
    }
  }

  if (screenChanged)
  {
    screenChanged = false;

    switch (screenIndex)
    {
      case SCREEN_TEMPERATURE:
        printTemperature(temp);
        break;

      case SCREEN_LEVEL:
        printWaterLevel(level);
        break;

      case SCREEN_PUMP:
        printPumpStatus();
        break;

      case SCREEN_HEATER:
        printHeaterStatus();
        break;

      case SCREEN_COOLER:
        printCoolerStatus();
        break;
    }
  }

  delay(100);
}

float getTemperature()
{
  Vout = Vin * ((float) (analogRead(TEMP)) / 1024.0);
  Rout = (Rt * Vout / (Vin - Vout));

  TempK = (beta / log(Rout / Rinf));
  return TempK - 273.15;  
}

long getWaterLevel()
{
  long t = 0, h = 0, p = 0;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  t = pulseIn(ECHO, HIGH);

  h = t / 58;
  h = HEIGHT - h;
  p = h * 100;
  p = p / HEIGHT;

  return p;
}

void printTemperature(long temp)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.print((char) 223);
  lcd.print("C");
}

void printWaterLevel(long level)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Water Level:");

  lcd.setCursor(0, 1);
  lcd.print(level);
  lcd.print((char) 37);
}

void printPumpStatus()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Pump Status:");

  lcd.setCursor(0, 1);
  lcd.print(pumpIsOn ? "ON" : "OFF");
}

void printHeaterStatus()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Heater Status:");

  lcd.setCursor(0, 1);
  lcd.print(heaterIsOn ? "ON" : "OFF");
}

void printCoolerStatus()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Cooler Status:");

  lcd.setCursor(0, 1);
  lcd.print(coolerIsOn ? "ON" : "OFF");
}

void processSerialInput()
{
  char ch = await();

  if (ch == 'p')
  {
    if (await() == 'i')
    {
      digitalWrite(PUMP, LOW);
      pumpIsOn = true;
    }
    else
    {
      digitalWrite(PUMP, HIGH);
      pumpIsOn = false;
    }

    return;
  }
  
  if (ch == 'h')
  {
    if (await() == 'i')
    {
      digitalWrite(HEATER, HIGH);
      heaterIsOn = true;
    }
    else
    {
      digitalWrite(HEATER, LOW);
      heaterIsOn = false;
    }

    return;
  }

  if (ch == 'c')
  {
    if (await() == 'i')
    {
      digitalWrite(COOLER, HIGH);
      coolerIsOn = true;
    }
    else
    {
      digitalWrite(COOLER, LOW);
      coolerIsOn = false;
    }

    return;
  }
}

