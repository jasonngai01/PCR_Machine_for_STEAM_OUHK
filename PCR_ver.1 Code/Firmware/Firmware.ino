
// Include Libraries
#include "Arduino.h"
#include "DS18B20.h"
#include "LiquidCrystal_PCF8574.h"
#include "Relay.h"


// Pin Definitions
#define DS18B20_1_PIN_DQ	2
#define DS18B20_2_PIN_DQ	3
#define RELAYMODULE1_1_PIN_SIGNAL	4
#define RELAYMODULE2_2_PIN_SIGNAL	5



// Global variables and defines
// There are several different versions of the LCD I2C adapter, each might have a different address.
// Try the given addresses by Un/commenting the following rows until LCD works follow the serial monitor prints. 
// To find your LCD address go to: http://playground.arduino.cc/Main/I2cScanner and run example.
#define LCD_ADDRESS 0x3F
//#define LCD_ADDRESS 0x27
// Define LCD characteristics
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255
// object initialization
DS18B20 ds18b20_1(DS18B20_1_PIN_DQ);
DS18B20 ds18b20_2(DS18B20_2_PIN_DQ);
LiquidCrystal_PCF8574 lcdI2C;
Relay relayModule1_1(RELAYMODULE1_1_PIN_SIGNAL);
Relay relayModule2_2(RELAYMODULE2_2_PIN_SIGNAL);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    // initialize the lcd
    lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT); 
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // DS18B20 1-Wire Temperature Sensor #1 - Test Code
    // Read DS18B20 temp sensor value in degrees celsius. for degrees fahrenheit use ds18b20_1.ReadTempF()
    float ds18b20_1TempC = ds18b20_1.readTempC();
    Serial.print(F("Temp: ")); Serial.print(ds18b20_1TempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '2') {
    // DS18B20 1-Wire Temperature Sensor #2 - Test Code
    // Read DS18B20 temp sensor value in degrees celsius. for degrees fahrenheit use ds18b20_2.ReadTempF()
    float ds18b20_2TempC = ds18b20_2.readTempC();
    Serial.print(F("Temp: ")); Serial.print(ds18b20_2TempC); Serial.println(F(" [C]"));

    }
    else if(menuOption == '3') {
    // LCD 16x2 I2C - Test Code
    // The LCD Screen will display the text of your choice.
    lcdI2C.clear();                          // Clear LCD screen.
    lcdI2C.print("  Circuito.io  ");                   // Print print String to LCD on first line
    lcdI2C.selectLine(2);                    // Set cursor at the begining of line 2
    lcdI2C.print("     Rocks!  ");                   // Print print String to LCD on second line
    delay(1000);

    }
    else if(menuOption == '4') {
    // Relay Module #1 - Test Code
    // The relay will turn on and off for 500ms (0.5 sec)
    relayModule1_1.on();       // 1. turns on
    delay(500);             // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    relayModule1_1.off();      // 3. turns off.
    delay(500);             // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }
    else if(menuOption == '5') {
    // Relay Module #2 - Test Code
    // The relay will turn on and off for 500ms (0.5 sec)
    relayModule2_2.on();       // 1. turns on
    delay(500);             // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    relayModule2_2.off();      // 3. turns off.
    delay(500);             // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) DS18B20 1-Wire Temperature Sensor #1"));
    Serial.println(F("(2) DS18B20 1-Wire Temperature Sensor #2"));
    Serial.println(F("(3) LCD 16x2 I2C"));
    Serial.println(F("(4) Relay Module #1"));
    Serial.println(F("(5) Relay Module #2"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing DS18B20 1-Wire Temperature Sensor #1"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing DS18B20 1-Wire Temperature Sensor #2"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing LCD 16x2 I2C"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing Relay Module #1"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing Relay Module #2"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/