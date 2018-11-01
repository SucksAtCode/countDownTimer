/* 
  countdown.pde
  Eric Ayars
  8/18/11

  Countdown timer 
  This program displays days, hours, minutes, and seconds until some
  specified time. At that time (and aftwards) it displays a message.
*/

#include <Wire.h>
#include <DS3231.h>
#include <avr/sleep.h>

#define SECONDSINDAY 86400
#define SECONDSINHOUR 3600
#define SECONDSINMINUTE 60

#define Max7219DIN 7  // Pin 7 connected to DIN (DataIN)
#define Max7219CLK 6  // Pin 6 connected to CLK
#define Max7219CS 5   // Pin 5 connected to CS
#include "LedControl.h"  // LedControl Library created by Eberhard Fahle at http://playground.arduino.cc/Main/LedControl

LedControl lc=LedControl(Max7219DIN,Max7219CLK,Max7219CS,1);   // Last number represent the number of Max7219 Modules connected 


unsigned long remSeconds, remMinutes, remHours, remDays;
byte year, month, day, hour, minute, second, DoW;

// Put your target date here. 
byte targetYear = 18; 
byte targetMonth = 11;
byte targetDay = 3;
byte targetHour = 19;
byte targetMinute = 45;
byte targetSecond = 0;

unsigned int monthdays[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

bool century, h24, ampm;
unsigned long targetDate;
unsigned long Now, dT;

DS3231 RTC;


unsigned long secondsSince2000(unsigned int year, byte month, byte day, byte hour, byte minute, byte second)
{
  // This code stolen from someene else's DS1337 library. It was called 
  // "date_to_epoch_seconds()" there, but it didn't do epoch seconds,
  // just seconds since 2000. That's fine in this case, since seconds
  // since 2000 is sufficient for this purpose.
  unsigned long sse = (((unsigned long)year)*365*24*60*60)   +   ((((unsigned long)year+3)>>2) + ((unsigned long)year%4==0 && (unsigned long)month>2))*24*60*60   +   \
         ((unsigned long)monthdays[month-1] + (unsigned long)day-1) *24*60*60   +   ((unsigned long)hour*60*60)   +   ((unsigned long)minute*60)   + (unsigned long)second;

  return sse;
}
// Variables to store individual numbers
int  firstnum=0;
int  secondnum=0;
int  thirdnum=0;
int  fournum=0;
int  fivenum=0;
int  sixnum=0;
int  sevennum=0;
int  eightnum=0;
long int countnumber=dT ; // Countdown timer start value HH:MM:SSSS

void setup() {
  Wire.begin();
  targetDate =secondsSince2000(targetYear, targetMonth, targetDay, targetHour, targetMinute, targetSecond);
  lc.shutdown(0,false);  // Wake up the display
  lc.setIntensity(0,15); // Set Brightness 0-15
  lc.clearDisplay(0);  // Clear display
  Serial.begin(9600);
}

void loop() {
  
  year = RTC.getYear();
  month = RTC.getMonth(century);
  if (century) year += 100;
  day = RTC.getDate();
  hour = RTC.getHour(h24, ampm);
  if ((h24) && (ampm)) hour += 12;
  minute = RTC.getMinute();
  second = RTC.getSecond();
  Now = secondsSince2000(year, month, day, hour, minute, second);

  // Are we past the countdown?
 // if (Now >= targetDate) {
 //   celebrate();
 // }

  // Here's the time remaining.
  dT = targetDate - Now;

  // Figure out whole days remaining
  remDays = dT/SECONDSINDAY;

  // Here's the leftover HMS
  remSeconds = dT%SECONDSINDAY;
  
  // Figure out the whole hours remaining
  remHours = remSeconds/SECONDSINHOUR;

  // Here's the leftover MS
  remSeconds = remSeconds%SECONDSINHOUR;

  // Figure out the whole minutes remaining
  remMinutes = remSeconds/SECONDSINMINUTE;

  // Here's the leftover S
  remSeconds = remSeconds%SECONDSINMINUTE;

  if (dT > -1) {
    firstnum = remDays/10%10;
    secondnum = remDays%10;
    thirdnum = remHours/10%10;
    fournum = remHours%10;
    fivenum = remMinutes/10%10;
    sixnum = remMinutes%10;
    sevennum = remSeconds/10%10;
    eightnum = remSeconds%10;
    Serial.print("remdays:");
    Serial.println(remDays);
    Serial.print("remHours:");
    Serial.println(remHours);
    Serial.print("remMintes:");
    Serial.println(remMinutes);
    Serial.print("remSeconds:");
    Serial.println(remSeconds);
    Serial.print("firstnum:");
    Serial.println(firstnum);
    Serial.print("secondnum:");
    Serial.println(secondnum);
    Serial.print("thirdnum:");
    Serial.println(thirdnum);
    Serial.print("fournum:");
    Serial.println(fournum);
    Serial.print("fivenum:");
    Serial.println(fivenum);
    Serial.print("sixnum:");
    Serial.println(sixnum);
    Serial.print("seventnum:");
    Serial.println(sevennum);
    Serial.print("eightnum:");
    Serial.println(eightnum);
    Serial.print("dT:");
    Serial.println(dT);
    Serial.print("Now:");
    Serial.println(Now);
    lc.setDigit(0,7,firstnum,false);
    lc.setDigit(0,6,secondnum,false);
    lc.setDigit(0,5,thirdnum,false);
    lc.setDigit(0,4,fournum,false);
    lc.setDigit(0,3,fivenum,false);
    lc.setDigit(0,2,sixnum,false);
    lc.setDigit(0,1,sevennum,false);
    lc.setDigit(0,0,eightnum,false);    
 /* }
  
  else {
      if (dT > 0) {
        firstnum = remDays/10%10;
    secondnum = remDays%10;
    thirdnum = remHours/10%10;
    fournum = remHours%10;
    fivenum = remMinutes/10%10;
    sixnum = remMinutes%10;
    sevennum = remSeconds/10%10;
    eightnum = remSeconds%10;
    Serial.println(firstnum);
    Serial.println(secondnum);
    Serial.println(remDays);
    Serial.println(thirdnum);
    Serial.println(fournum);
    Serial.println(remHours);
    Serial.println(fivenum);
    Serial.println(sixnum);
    Serial.println(remMinutes);
    Serial.println(sevennum);
    Serial.print(eightnum);
    Serial.println(remSeconds);
    Serial.println(dT);
    Serial.println(Now);
    lc.setDigit(0,7,firstnum,false);
    lc.setDigit(0,6,secondnum,false);
    lc.setDigit(0,5,thirdnum,false);
    lc.setDigit(0,4,fournum,false);
    lc.setDigit(0,3,fivenum,false);
    lc.setDigit(0,2,sixnum,false);
    lc.setDigit(0,1,sevennum,false);
    lc.setDigit(0,0,eightnum,false);   
*/
      }
      else 
      if (dT < -2){
       
    firstnum = remDays/10%10;
    secondnum = remDays%10;
    thirdnum = remHours/10%10;
    fournum = remHours%10;
    fivenum = remMinutes/10%10;
    sixnum = remMinutes%10;
    sevennum = remSeconds/10%10;
    eightnum = remSeconds%10;
  
                  Serial.println(firstnum);
    Serial.print(secondnum);
    Serial.println(remDays);
    Serial.println(thirdnum);
    Serial.print(fournum);
    Serial.println(remHours);
    Serial.println(fivenum);
    Serial.print(sixnum);
    Serial.println(remMinutes);
    Serial.println(sevennum);
    Serial.print(eightnum);
    Serial.println(remSeconds);
    Serial.println(dT);
    Serial.println(Now);
        lc.setDigit(0,7,firstnum,false);
    lc.setDigit(0,6,secondnum,false);
    lc.setDigit(0,5,thirdnum,false);
    lc.setDigit(0,4,fournum,false);
    lc.setDigit(0,3,fivenum,false);
    lc.setDigit(0,2,sixnum,false);
    lc.setDigit(0,1,sevennum,false);
    lc.setDigit(0,0,eightnum,false);   
             }
  /* Make sure we're dealing with a blank display
  lcd.clear();

  // Print the top line to the display
  lcd.setCursor(0,0);
  lcd.print(remDays, DEC);
  lcd.setCursor(4,0);
  lcd.print("Days ");
  lcd.setCursor(9,0);
  lcd.print(remHours, DEC);
  lcd.setCursor(12,0);
  lcd.print("Hrs ");

  // Print the bottom line to the display
  lcd.setCursor(0,1);
  lcd.print(remMinutes, DEC);
  lcd.setCursor(4,1);
  lcd.print("Min ");
  lcd.setCursor(9,1);
  lcd.print(remSeconds, DEC);
  lcd.setCursor(12,1);
  lcd.print("Sec ");
 */
  // No need running this more than 4x per second, really... 
  delay(1000);
  }
//      }
