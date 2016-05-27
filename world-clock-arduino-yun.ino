/*
   World Clock Yun
   for Arduino Yun (using OpenWRT bridge)
   
   Yu-Sung Chang
*/

#include <LiquidCrystal.h>
#include <Process.h>

// For OSEPP-LCD-01
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// For regular LCD
//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

Process date;


#define localeNUMBER 6 /* Number of locations */
String locale_names[localeNUMBER] = {"San Francisco", "Chicago", "New York", "London", "Hong Kong", "Seoul"};
String locale_tz[localeNUMBER] = {"PST8PDT", "CST6CDT", "EST5EDT", "GMT0BST", "HKT-8", "KST-9"}; /* Time Zone */

#define screenTIME 3000 /* How long each screen will stay in milliseconds */
int screen = 0;
int screen_prev = -1;
long timer_current = 0;
long timer_start = 0;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("World Clock");
  lcd.setCursor(0, 1);
  lcd.print("Synchronizing..");
  lcd.blink();
  
  Bridge.begin();

  lcd.noBlink();
}

void loop()
{
  timer_current = millis();
   
  if (timer_start == 0) timer_start = timer_current;

  if (timer_current - timer_start > screenTIME)
  {
    screen = (screen + 1) % localeNUMBER;
    timer_start = timer_current;
  }

   date.runShellCommand("TZ=" + locale_tz[screen] + " date +'%r %a   '");
  
  lcd.setCursor(0, 0);
  lcd.print(date.readString());
  date.flush();

  if (screen != screen_prev)
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(locale_names[screen]);
    
    screen_prev = screen;
  }
}

