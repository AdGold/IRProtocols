#include <IRProtocols.h>
#include <LiquidCrystal.h>
#include <stdarg.h>

#define CLOCK 10
#define LATCH 11
#define DATA 12
#define LEDPin 13

void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
        Serial.print(tmp);
}

void led(int n)
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, n);
  digitalWrite(LATCH, HIGH);
}

LiquidCrystal lcd(6,7,8,2,3,4,5);
IRHeliXTrust ir(16);
//IRHeliGyro ir(16);
//IRRemoteStandard ir(16);

void setup()
{
  Serial.begin(115200);
  lcd.begin(16,2);
  ir.begin();
  pinMode(LEDPin, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
}

void loop()
{
  byte bytes[10];
  int data[15];
  if (ir.poll(-1))
  {
    if (ir.getToggle())
      p("T: %d, Y: %d, P: %d, tr: %d, ch: %d, mo: %d\n", ir.getThrottle(), ir.getPitch(), ir.getYaw(), ir.getTrim(), ir.getChannel(), ir.getMode());
    int upto;
    switch(ir.getChannel())
    {
    case 1:
      lcd.home();
      lcd.print(ir.getThrottle());
      lcd.print("   ");
      lcd.setCursor(8,0);
      lcd.print(ir.getPitch());
      lcd.print("   ");
      lcd.setCursor(0,1);
      lcd.print(ir.getYaw());
      lcd.print("   ");
      lcd.setCursor(8,1);
      lcd.print(ir.getTrim());
      lcd.print("   ");
      digitalWrite(LEDPin, ir.getMode());
      led((ir.getToggle())?0xFF:0);
    break;
    case 2:
      upto = map(ir.getThrottle(), 0, 63, 0, 8);
      led((1<<upto)-1);
    break;
    case 3:
    break;
    }
    //p("T: %d, Y: %d, P: %d, t: %d, ch: %d\n", ir.getThrottle(), ir.getPitch(), ir.getYaw(), ir.getTrim(), ir.getChannel());
    //p("m: %d, b: %d\n", ir.getModel(), ir.getButton());
  }
  return;
  if (ir.getData(data, bytes, -1))
  {
    for (int i = 0; i < ir.getNFields(); i++)
    {
      Serial.print(data[i]);
      Serial.print(' ');
    }
    Serial.println();
  }
}
