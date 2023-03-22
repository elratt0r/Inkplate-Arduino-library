/*
   Inkplate5_RTC_Simple example for Soldered Inkplate 5
   For this example you will need USB cable and Inkplate 5.
   Select "Soldered Inkplate5" from Tools -> Board menu.
   Don't have "Soldered Inkplate5" option? Follow our tutorial and add it:
   https://soldered.com/learn/add-inkplate-6-board-definition-to-arduino-ide/

   Example shows how to use basic clock functions of PCF85063A RTC on Inkplate board.
   This example will show how to set time and date, how to read time and how to print time on Inkplate using partial
   updates. NOTE: Partial update is only available on 1 Bit mode (BW) and it is not recommended to use it on first
   refresh after power up. It is recommended to do a full refresh every 5-10 partial refresh to maintain good picture
   quality.

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: https://forum.soldered.com/
   21 March 2023 by Soldered
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE5
#error "Wrong board selection for this example, please select Soldered Inkplate5 in the boards menu."
#endif

#include "Inkplate.h"            // Include Inkplate library to the sketch
Inkplate display(INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1-bit mode (BW)

#define REFRESH_DELAY 1000 // Delay between refreshes
unsigned long time1;       // Time for measuring refresh in millis

// Set clock
uint8_t hours = 8;
uint8_t minutes = 25;
uint8_t seconds = 0;

// Set date and weekday (NOTE: In weekdays 0 means Sunday, 1 menas Monday, ...)
uint8_t weekday = 4;
uint8_t day = 21;
uint8_t month = 3;
uint8_t year = 23;

void setup()
{
    display.begin();        // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay(); // Clear frame buffer of display
    display.display();      // Put clear image on display
    display.setTextSize(4); // Set text to be 4 times bigger than classic 5x7 px text

    display.rtcSetTime(hours, minutes, seconds);   // Send time to RTC
    display.rtcSetDate(weekday, day, month, year); // Send date to RTC
}

// Variable that keeps count on how much screen has been partially updated
int n = 0;

void loop()
{
    if ((unsigned long)(millis() - time1) > REFRESH_DELAY)
    {
        display.rtcGetRtcData();           // Get the time and date from RTC
        seconds = display.rtcGetSecond();  // Store senconds in a variable
        minutes = display.rtcGetMinute();  // Store minutes in a variable
        hours = display.rtcGetHour();      // Store hours in a variable
        weekday = display.rtcGetWeekday(); // Store day of week in a variable
        day = display.rtcGetDay();         // Store day of month in a variable
        month = display.rtcGetMonth();     // Store month in a variable
        year = display.rtcGetYear();       // Store year in a variable

        display.clearDisplay();      // Clear content in frame buffer
        display.setCursor(140, 260); // Set position of the text
        printTime(hours, minutes, seconds, day, weekday, month, year); // Print the time on screen

        if (n > 9) // Check if you need to do full refresh or you can do partial update
        {
            display.display(true); // Do a full refresh
            n = 0;
        }
        else
        {
            display.partialUpdate(false, true); // Do partial update and keep e-papr power supply on
            n++;                                // Keep track on how many times screen has been partially updated
        }

        time1 = millis(); // Store current millis
    }
}

void printTime(uint8_t _hour, uint8_t _minutes, uint8_t _seconds, uint8_t _day, uint8_t _weekday, uint8_t _month,
               uint16_t _year)
{
    // Write time and date info on screen
    char *wday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    print2Digits(_hour);
    display.print(':');
    print2Digits(_minutes);
    display.print(':');
    print2Digits(_seconds);

    display.print(' ');

    display.print(wday[_weekday]);
    display.print(", ");
    print2Digits(_day);
    display.print('/');
    print2Digits(_month);
    display.print('/');
    display.print(_year, DEC);
}

void print2Digits(uint8_t _d)
{
    if (_d < 10)
        display.print('0');
    display.print(_d, DEC);
}
