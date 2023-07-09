/*
   Inkplate4_Wake_Up_Button example for Soldered Inkplate 4
   For this example you will need USB cable and an Inkplate 4
   Select "Soldered Inkplate 4" from Tools -> Board menu.
   Don't have "Soldered Inkplate 4" option? Follow our tutorial and add it:
   https://soldered.com/learn/add-inkplate-6-board-definition-to-arduino-ide/

   Here is shown how to use ESP interrupts to wake up the MCU from deepsleep when wake up button
   is pressed. Also, wake up on timer after 30 seconds of deep sleep if the button is not pressed.

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: https://forum.soldered.com/
   8 Jul 2023 by Soldered
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE4
#error "Wrong board selection for this example, please select Soldered Inkplate4 in the boards menu."
#endif

// Include Inkplate library
#include <Inkplate.h>

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000
// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP 30

// Initiate Inkplate object
Inkplate display;

// Store int in rtc data, to remain persistent during deep sleep
RTC_DATA_ATTR int bootCount = 0;

void setup()
{
    display.begin();

    ++bootCount;

    // Our function declared below
    displayInfo();

    // Go to sleep for TIME_TO_SLEEP seconds
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    // Enable wakeup from deep sleep on gpio 36 (wake button)
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, LOW);

    // Go to sleep
    esp_deep_sleep_start();
}

void loop()
{
    // Never here! If you use deep sleep, the whole program should be in setup() because the board restarts each
    // time. loop() must be empty!
}

// Function that will write number of boots and boot reason to screen
void displayInfo()
{
    // First, lets delete everything from frame buffer
    display.clearDisplay();

    // Set text cursor and size
    display.setCursor(130, 100);
    display.setTextSize(2);

    display.print(F("Boot count: "));
    display.println(bootCount, DEC); // Print the number

    // Set next line cursor position and text size
    display.setCursor(122, 140);
    display.setTextSize(0);

    // Display wake up reason
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        display.println("Wakeup caused by WakeUp button");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        display.println("Wakeup caused by timer");
        break;
    default:
        display.println("Wakeup was not caused by deep sleep");
        break;
    }

    display.display();
}
