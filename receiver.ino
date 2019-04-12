/*
 SMS receiver

 This sketch, for the MKR GSM 1400 board, waits for a SMS message
 and displays it through the Serial port.

 Circuit:
 * MKR GSM 1400 board
 * Antenna
 * SIM card that can receive SMS messages

 created 25 Feb 2012
 by Javier Zorzano / TD
*/

// include the GSM library
#undef max
#undef min

#include <iostream>
#include <FlashAsEEPROM.h>
#include <MKRGSM.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include "arduino_secrets.h"

#include <string>

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];

void setup()
{

    // initialize serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Wire.begin();

    oled.init();         // Initialze SSD1306 OLED display
    oled.clearDisplay(); // Clear screen
                         //    oled.setFont(font5x7); // Set font type (default 8x8)

    oled.setTextXY(0, 0); // Set cursor position, start of line 0
    oled.putString(" Harmony wallet ");
    oled.setTextXY(1, 0); // Set cursor position, start of line 1
    oled.putString("tel:+31622167828");

    // connection state
    bool connected = false;

    // Start GSM connection
    while (!connected)
    {
        Serial.println("connecting..");
        if (gsmAccess.begin(SECRET_PINNUMBER) == GSM_READY)
        {
            connected = true;
        }
        else
        {
            Serial.println("Not connected");
            delay(1000);
        }
    }
    oled.setTextXY(2, 0);
    oled.putString("   GSM ready!   ");
}

void loop()
{
    int c;

    // If there are any SMSs available()
    if (sms.available())
    {
        Serial.println("Message received from:");

        // Get remote number
        // sms.remoteNumber(senderNumber, 20);
        // oled.putString(senderNumber);

        // An example of message disposal
        // Any messages starting with # should be discarded
        if (sms.peek() == '#')
        {
            // Serial.println("Discarded SMS");
            sms.flush();
        }

        // Read message bytes and print them
        {
            // oled.setTextXY(0, 0);
            while ((c = sms.read()) != -1)
            {
                const char *msg = (const char *)&c;
                oled.setTextXY(2, 0); // Set cursor position, start of line 2
                oled.putString(msg);
            }
        }

        // Serial.println("\nEND OF MESSAGE");

        // Delete message from modem memory
        sms.flush();
        // Serial.println("MESSAGE DELETED");
    }

    delay(1000);
}
