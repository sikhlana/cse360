#include <LiquidCrystal.h>

/* LCD */
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

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

boolean isError = false;

void setup()
{
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.print("Initializing...");

  beta = (log(RT1 / RT2)) / ((1 / T1) - (1 / T2));
  Rinf = R0 * exp(-beta / T0);

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

  printTemperature(temp);
  delay(2000);

  temp = getTemperature();
  Serial.println(temp);
  
  level = getWaterLevel();
  Serial.println(level);

  delay(20);

  while (Serial.available() >= 1)
  {
    processSerialInput();
  }
  
  printWaterLevel(level);
  delay(2000);

  while (Serial.available() >= 1)
  {
    processSerialInput();
  }
}

float getTemperature()
{
  Vout = Vin * ((float) (analogRead(TEMP)) / 1024.0);
  Rout = (Rt * Vout / (Vin - Vout));

  TempK = (beta / log(Rout / Rinf));
  return TempK - 273.15;  
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

void printWaterLevel(long level)
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Water Level:");

  lcd.setCursor(0, 1);
  lcd.print(level);
  lcd.print((char) 37);
}

void processSerialInput()
{
  char ch = await();
  
  if (ch == 'h')
  {
    if (await() == 'i')
    {
      digitalWrite(HEATER, HIGH);
    }
    else
    {
      digitalWrite(HEATER, LOW);
    }

    return;
  }

  if (ch == 'c')
  {
    if (await() == 'i')
    {
      digitalWrite(COOLER, HIGH);
    }
    else
    {
      digitalWrite(COOLER, LOW);
    }

    return;
  }

  if (ch == 'p')
  {
    if (await() == 'i')
    {
      digitalWrite(PUMP, LOW);
    }
    else
    {
      digitalWrite(PUMP, HIGH);
    }

    return;
  }
}

