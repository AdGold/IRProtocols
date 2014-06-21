#include <IRProtocols.h>
#include <LiquidCrystal.h>
#include <stdarg.h>

void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
        Serial.print(tmp);
}

//IRHeliXTrust ir(16);
//IRHeliGyro ir(16);
IRRemoteStandard ir(16);
LiquidCrystal lcd(6,7,8,2,3,4,5);

void setup()
{
  Serial.begin(115200);
  lcd.begin(16,2);
  ir.begin();
}

void loop()
{
  if (ir.poll(-1))
  {
    //p("T: %d, Y: %d, P: %d, tr: %d, ch: %d, mo: %d, tog: %d\n", ir.getThrottle(), ir.getPitch(), ir.getYaw(), ir.getTrim(), ir.getChannel(), ir.getMode(), ir.getToggle());
    //p("%d %d %d %d %d %d %d\n", ir.getThrottle(), ir.getPitch(), ir.getYaw(), ir.getTrim(), ir.getChannel(), ir.getMode(), ir.getToggle());
    //p("T: %d, Y: %d, P: %d, t: %d, ch: %d\n", ir.getThrottle(), ir.getPitch(), ir.getYaw(), ir.getTrim(), ir.getChannel());
    p("%d %d\n", ir.getModel(), ir.getButton());
    lcd.clear();
    lcd.print(ir.getModel());
    lcd.setCursor(0,1);
    lcd.print(ir.getButton());
  }
}
