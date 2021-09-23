/*
   Inkplate_RTC_Alarm_Interrupt_Sleep_Example example for e-radionica Inkplate 6PLUS
   For this example you will need only USB cable and Inkplate 6PLUS
   Select "Inkplate 6PLUS(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 6PLUS(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   This example will show you how to use RTC alarm interrupt and sleep.
   You can use this example to wake up your ESP32 from deep sleep.
   Inkplate features RTC chip with interrupt for alarm connected to GPIO39

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   19 August 2021 by e-radionica.com
*/

#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6PLUS in the boards menu."
#endif

#include "Inkplate.h"      // Include Inkplate library to the sketch
#include "driver/rtc_io.h" // Include ESP32 library for RTC pin I/O (needed for rtc_gpio_isolate() function)
#include <rom/rtc.h>       // Include ESP32 library for RTC (needed for rtc_get_reset_reason() function)

#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  10      // Time how long ESP32 will be in deep sleep (in seconds). In this case 10 seconds.

Inkplate display(INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1-bit mode (BW)

void setup()
{
    Serial.begin(115200);
    display.begin();

    //  setTime(hour, minute, sec);
    display.rtcSetTime(6, 54, 00); // 24H mode, ex. 6:54:00
    //  setDate(weekday, day, month, yr);
    display.rtcSetDate(6, 16, 5, 2020); // 0 for Sunday, ex. Saturday, 16.5.2020.
    //  setAlarm(alarm_second, alarm_minute, alarm_hour, alarm_day, alarm_weekday);
    display.rtcSetAlarm(15, 54, 99, 99, 99); // use 99 if no alarm
    checkAlarm();
}

void loop()
{
    display.setCursor(100, 100);
    display.setTextSize(3);

    display.println("Now is:");
    printCurrentTime();

    display.display();

    display.println("Entering sleep mode in 1 second");
    display.partialUpdate();

    delay(1000);

    sleepNow(); // sleep function called here
}

void printCurrentTime()
{
    display.setCursor(100, display.getCursorY());
    display.setTextSize(3);

    switch (display.rtcGetWeekday())
    {
    case 0:
        display.print("Sunday , ");
        break;
    case 1:
        display.print("Monday , ");
        break;
    case 2:
        display.print("Tuesday , ");
        break;
    case 3:
        display.print("Wednesday , ");
        break;
    case 4:
        display.print("Thursday , ");
        break;
    case 5:
        display.print("Friday , ");
        break;
    case 6:
        display.print("Saturday , ");
        break;
    }

    display.print(display.rtcGetDay());
    display.print(".");
    display.print(display.rtcGetMonth());
    display.print(".");
    display.print(display.rtcGetYear());
    display.print(". ");
    display.print(display.rtcGetHour());
    display.print(":");
    display.print(display.rtcGetMinute());
    display.print(":");
    display.println(display.rtcGetSecond());
}

void checkAlarm()
{
    display.setCursor(100, 400);
    display.setTextSize(3);
    display.print("Alarm is set to match: ");
    switch (display.rtcGetAlarmWeekday())
    {
    case 0:
        display.print("Sunday , ");
        break;
    case 1:
        display.print("Monday , ");
        break;
    case 2:
        display.print("Tuesday , ");
        break;
    case 3:
        display.print("Wednesday , ");
        break;
    case 4:
        display.print("Thursday , ");
        break;
    case 5:
        display.print("Friday , ");
        break;
    case 6:
        display.print("Saturday , ");
        break;
    default:
        break; // for getAlarmWeekday=99 alarm is not set for a weekday, do not print
    }

    if (display.rtcGetAlarmDay() != 99)
    {
        display.print("Date: ");
        display.print(display.rtcGetAlarmDay());
    }
    if (display.rtcGetAlarmHour() != 99)
    {
        display.print(" hour: ");
        display.print(display.rtcGetAlarmHour());
    }
    if (display.rtcGetAlarmMinute() != 99)
    {
        display.print(" minute: ");
        display.print(display.rtcGetAlarmMinute());
    }
    if (display.rtcGetAlarmSecond() != 99)
    {
        display.print(" second: ");
        display.print(display.rtcGetAlarmSecond());
    }

    display.println("");
}

void sleepNow()
{
    // Enable wakup from deep sleep on gpio 39 where RTC interrupt is connected
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);

    // Go to sleep
    esp_deep_sleep_start();
}