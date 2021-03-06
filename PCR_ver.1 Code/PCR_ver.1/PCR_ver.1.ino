/********************************************************
 * PCR Machine Arduino Code
 * In this program, it allows users to define the Temperture 
 * , Duratuation of different stage and number of cycles. 
 * It is helpful to make copies of different samples DNA with 
 * different specification. 
 * Please check the mechanical design and the electrontic
 * diagram in the Github. 
 * https://github.com/jasonngai01/PCR_Machine_for_STEAM
 ********************************************************/

// Inport the library for driving the Temperauture sensor (DS18B20) and LiquidCrystal display (IC2 1602)

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,16,2);  

// Create the Variable for storing the User Input and Sensor message
int denatTime;          
int denatTemp;          
int annealTime;
int annealTemp;
int extendTime;
int extendTemp;
int cycles;        
int finalannealTemp; 
int decreTemp;  
unsigned long now = millis ();
int heater = 2; // D2 Pin to Control Relay, Heater relay setup is ALWAY ON
int cooler = 6; //D6 Pin to Control Relay, Cooler relay setup is ALWAY OFF
int time = 0;   // Time counter for cycle timing
int cycle = 0;  // Cycle counter
float tempC0;
float tempC1;
float tempC_avg; // The average temperature from the sensors.
int deviceCount = 0;

 
// Data (Temperature) wire is plugged into pin 4 on the Arduino
#define ONE_WIRE_BUS 4
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  //Setup Temperature sensor
  deviceCount = sensors.getDeviceCount();
  Serial.print("Locating Temperature devices...");
  Serial.print("Found ");
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  //Setup Relay module
  pinMode(heater, OUTPUT);  // Set Pin connected to Relay as an OUTPUT
  pinMode(cooler, OUTPUT);  // Set Pin connected to Relay as an OUTPUT
  digitalWrite(heater, HIGH);  // Set Pin to HIGH to turn Relay OFF >> Testing 
  digitalWrite(cooler, HIGH);  // Set Pin to LOW to turn Relay ON >> Testing 
  delay(1080);
  digitalWrite(heater, HIGH);  // Set Pin to HIGH to turn Relay OFF
  digitalWrite(cooler, LOW);  // Set Pin to LOW to turn Relay OFF
  
  //Setup IC2 LCD
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

  // Print a message on both lines of the LCD.
  lcd.setCursor(3,0);   //Set cursor to character 2 on line 0
  lcd.print("OUHK STEAM");
  
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("PCR Thermocycler");

  delay(1080);


 
}


void loop(void)
{
  //In the Initial Stage, User need to input the recipe for PCR heating and cooling.
  Serial.println("Please input all the followings parameters for the PCR. ");
  Serial.println("1. Enter the Denaturation Time in seconds: ");        
  while (Serial.available()==0)  {}
  denatTime=Serial.parseInt();  
  Serial.println(denatTime);                                        
  //

  //
  Serial.println("2. Enter the Denaturation Temp in Celsius "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  denatTemp=Serial.parseInt();                                      
  Serial.println(denatTemp); 
  //Read user input into age

  //
  Serial.println("3. Enter the Annealing Time in seconds "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  annealTime=Serial.parseInt();                                      
  Serial.println(annealTime);  
  //

  //
  Serial.println("4. Enter the Annealing Temp in Celsius "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  annealTemp=Serial.parseInt();                                     
  Serial.println(annealTemp);  
  //

  //
  Serial.println("5. Enter the Extension Time in seconds "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  extendTime=Serial.parseInt();                                      
  Serial.println(extendTime);  
  //

  //
  Serial.println("6. Enter the Extension Temp in Celsius "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  extendTemp=Serial.parseInt();                                      
  Serial.println(extendTemp);
  //
  
  //
  Serial.println("7. Enter the number of cycles "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  cycles=Serial.parseInt();                                      
  Serial.println(cycles);
  //
  
  //
  Serial.println("8. Enter the Final Annealing Temp in Celsius "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  finalannealTemp=Serial.parseInt();                                      
  Serial.println(finalannealTemp);   
  //Read user input into age

  //
  Serial.println("9. Enter the Temp decreases per cycle in Celsius "); 
  now = millis ();
  while (millis () - now < 1000)
  Serial.read ();  // read and discard any input
  while (Serial.available()==0)  {}
  decreTemp=Serial.parseInt();                                      
  Serial.println(decreTemp);                                                                           
  //All the parameters of recipe for PCR heating and cooling are received. 
  Serial.println("All the parameters for the PCR are already inputted. "); 
  delay(1080);
  Serial.println("The PCR process is running now. "); 
  delay(1080);
  
  // Warm up stage: Heating to the denatTemp
  sensors.requestTemperatures(); 
  Serial.println("**********************************************");
  Serial.println("Warming up.");
  Serial.println("This may take several minutes.");
  Serial.println("**********************************************");
  Serial.print("Heating to ");
  Serial.print(denatTemp);
  Serial.println("C");
  delay(1080);
  
  while (sensors.getTempCByIndex(0) < denatTemp ||sensors.getTempCByIndex(1)< denatTemp ) {      //***95*** While Loop to Bring up to Temp - 95C                  
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print("Temperature is: ");
    Serial.println((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2);
    // Print a message on both lines of the LCD.
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("PCR Thermocycler");
    lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
    lcd.print("Temperature: ");
    lcd.setCursor(12,1);   //Move cursor to character 2 on line 1
    lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    digitalWrite(heater, LOW);  // Heating 
    digitalWrite(cooler, LOW);  
    delay(360);
  }
 
    
  //Phase 1: Stay at 94C for 180 sec
  Serial.println("Intialization Phase (180 sec).");
  while ( time < 180) {  // ***Set to 180*** While Loop to keep temp at 95C for 180 seconds         
    
    Serial.println("Maintain at 94 Celsius for 180 seconds");
    Serial.print(time);
    Serial.println(" out of 180 seconds");
    time = time + 1;
    delay(360);
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) > denatTemp-4 ||sensors.getTempCByIndex(1) > denatTemp-4 ) {                                                                         
         digitalWrite(heater, HIGH); // Stop Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("PCR Thermocycler");
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 2 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
                                          }                                                                       
    else {                                                                         
         digitalWrite(heater, LOW); // Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("PCR Thermocycler");
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 2 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         }                                                                        
  }   
  
//Cycle Counter
//The cycle loops starts here. 
//The loop starts from Phase 2 and ends to Phase 7. 
while (cycle < cycles) {      
  cycle = cycle + 1;
  Serial.println("**********************************************");
  Serial.print(cycle);
  Serial.print(" of ");
  Serial.print(cycles);
  Serial.println(" cycles");
// Print a message on both lines of the LCD.
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Cycle:");
  lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
  lcd.print(cycle);
  lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
  lcd.print("of");
  lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
  lcd.print(cycles);
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("Temperature: ");
  lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
  lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));

  
  //Phase 2: Increase Temp to 94C  
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("**********************************************");
  Serial.print("Phase 2: Set to ");
  Serial.print(denatTemp);
  Serial.println("C");

  while (sensors.getTempCByIndex(0) < denatTemp-1 || sensors.getTempCByIndex(1) < denatTemp-1) {      //***94*** While Loop to Bring up to Temp - 94C                  
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print("Temperature is: ");
    Serial.println((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2);
    // Print a message on both lines of the LCD.
    lcd.init();
    lcd.clear();         
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("Cycle:");
    lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
    lcd.print(cycle);
    lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
    lcd.print("of");
    lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
    lcd.print(cycles);
    lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
    lcd.print("Temperature: ");
    lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
    lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    digitalWrite(heater, LOW); // Heating
    digitalWrite(cooler, LOW);
    delay(360);
  }

  
  //Phase 3: Hold at 95C for Set Denaturation Time in sec Cycle  
  time=0;
  Serial.println("**********************************************");
  Serial.print("Temp ");
  Serial.print(denatTemp);
  Serial.print("C for ");
  Serial.print(denatTime);
  Serial.println(" seconds");
  Serial.println("**********************************************");
  while ( time < denatTime) {   // Hold at 95C.
    Serial.print(cycle);
    Serial.print(" of ");
    Serial.print(cycles);
    Serial.print(" cycles, ");
    Serial.print("Phase 3: Hold at ");
    Serial.print(denatTemp);
    Serial.print("C, ");
    Serial.print(time);
    Serial.print(" out of ");
    Serial.print(denatTime);
    Serial.println(" seconds");
    time = time + 1;
    delay(360);
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) > denatTemp-1 || sensors.getTempCByIndex(1) > denatTemp-1) {                                                                         
         digitalWrite(heater, HIGH); // Stop Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
                                          }                                                                       
    else {                                                                         
         digitalWrite(heater, LOW); // Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         }                                                                        
  }   
 
//Phase 4: Drop Temp to 53C  
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("########");
  Serial.print("Phase 4: Set to ");
  Serial.print(annealTemp);
  Serial.println("C");
  delay(720);
  
  while (sensors.getTempCByIndex(0) > annealTemp+12 || sensors.getTempCByIndex(1) > annealTemp+12) {      //***53*** While Loop to Drop Temp - 53C                  
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print("Temperature is: ");
    Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    digitalWrite(heater, HIGH); // Stop Heating
    digitalWrite(cooler, HIGH); // Cooling
     // Print a message on both lines of the LCD.
    lcd.init();
    lcd.clear();         
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("Cycle:");
    lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
    lcd.print(cycle);
    lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
    lcd.print("of");
    lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
    lcd.print(cycles);
    lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
    lcd.print("Temperature: ");
    lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
    lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    delay(360);
  }

//Phase 5: Hold at 53C for Set Annealing Time in sec Cycle  
  Serial.println("**********************************************");
  Serial.print("Phase 5: Hold at ");
  Serial.print(annealTemp);
  Serial.print("C for ");
  Serial.print(annealTime);
  Serial.println(" seconds");
  Serial.println("**********************************************");
  delay(720);
  time=0;
  while ( time < annealTime) {   // Hold at 53C.
    Serial.print(cycle);
    Serial.print(" of ");
    Serial.print(cycles);
    Serial.print(" cycles, ");
    Serial.print("Phase 5: Hold at ");
    Serial.print(annealTemp);
    Serial.print("C, ");
    Serial.print(time);
    Serial.print(" out of ");
    Serial.print(annealTime);
    Serial.println(" seconds");
    time = time + 1;
    delay(360);
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) > annealTemp+12 || sensors.getTempCByIndex(1) > annealTemp+12 ) {                                                                         
         digitalWrite(heater, HIGH); // Stop Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
                                          }        
                                                                                                         
    else {                                                                         
         digitalWrite(heater, LOW); // Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         }                                                                        
  }   


//Phase 6: Increase Temp to 72C  
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("**********************************************");
  Serial.print("Phase 6: Set to ");
  Serial.print(extendTemp);
  Serial.println("C");
  delay(720);

  while (sensors.getTempCByIndex(0) < extendTemp+2 || sensors.getTempCByIndex(1) < extendTemp+2 ) {      //***72*** While Loop to Bring up to Temp - 72C                  
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.print("Temperature is: ");
    Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    digitalWrite(heater, LOW); // Heating
    digitalWrite(cooler, LOW);
    // Print a message on both lines of the LCD.
    lcd.init();
    lcd.clear();         
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("Cycle:");
    lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
    lcd.print(cycle);
    lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
    lcd.print("of");
    lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
    lcd.print(cycles);
    lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
    lcd.print("Temperature: ");
    lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
    lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
    delay(360);
  }

//Phase 7: Hold at 72C for set Extension Time in sec Cycle  
  Serial.println("**********************************************");
  Serial.print("Phase 7: Hold at ");
  Serial.print(extendTemp);
  Serial.print("C for ");
  Serial.print(extendTime);
  Serial.println(" seconds");
  Serial.println("**********************************************");
  delay(720);
  time=0;
  while ( time < extendTime) {   // Hold at 72C.
    Serial.print(cycle);
    Serial.print(" of ");
    Serial.print(cycles);
    Serial.print(" cycles, ");
    Serial.print("Phase 7: Hold at ");
    Serial.print(extendTemp);
    Serial.print("C, ");
    Serial.print(time);
    Serial.print(" out of ");
    Serial.print(extendTime);
    Serial.println(" seconds");
    time = time + 1;
    delay(360);
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) > extendTemp+2 ||sensors.getTempCByIndex(1) > extendTemp+2 ) {                                                                         
         digitalWrite(heater, HIGH); // Stop Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
                                          }                                                                       
    else {                                                                         
         digitalWrite(heater, LOW); // Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         Serial.println(" ");
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Cycle:");
         lcd.setCursor(7,0);   //Move cursor to character 6 on line 0
         lcd.print(cycle);
         lcd.setCursor(10,0);   //Set cursor to character 2 on line 0
         lcd.print("of");
         lcd.setCursor(13,0);   //Move cursor to character 6 on line 0
         lcd.print(cycles);
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         }                                                                        
  }
  
  //The annealTemp is going to decrease in decreTemp in every cycle until to the finalannealTemp.
  //For example, if the annealTemp is 53C, the decreTemp is 1C and the finalannealTemp is 50C.
  //There are total 20 cycles. In cycle 1, the annealTemp is 53C. In cycle 2, the annealTemp is 52C.
  //In cycle 3, the annealTemp is 51C. In cycle 4, the annealTemp is 50C.
  //In cycle 5 to cycle 20, it is already achieved finalannealTemp. It will keep 50C. 
  
  if (annealTemp - decreTemp > finalannealTemp) {
    annealTemp = annealTemp - decreTemp;
  }
  else {
  annealTemp = finalannealTemp;
  }

} //The cycle loops end here. 

//Phase 8: Hold at 72C for 10 min  
  Serial.println("**********************************************");
  Serial.print("Phase 8: Hold at ");
  Serial.print(extendTemp);
  Serial.print("C for ");
  Serial.println("10 minutes");
  Serial.println("**********************************************");
  delay(720);
  time=0;
  while ( time < 600) {   // Hold at 72C for 600 sec.
    Serial.print("Phase 8: Hold at ");
    Serial.print(extendTemp);
    Serial.println(" Celsius");
    Serial.println(time);
    time = time + 1;
    delay(360);
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) > extendTemp-5 || sensors.getTempCByIndex(1) > extendTemp-5 ) {                                                                         
         digitalWrite(heater, HIGH); // Stop Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Final  Extension");
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
                                          }                                                                       
    else {                                                                         
         digitalWrite(heater, LOW); // Heating
         digitalWrite(cooler, LOW);
         Serial.print("Current temperature: ");
         Serial.println(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         // Print a message on both lines of the LCD.
         lcd.init();
         lcd.clear();         
         lcd.backlight();      // Make sure backlight is on
         lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
         lcd.print("Final  Extension");
         lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
         lcd.print("Temperature: ");
         lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
         lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));
         }                                                                        
  }   

  
//Phase 9: Final cooling stage 
// Print a message on both lines of the LCD.
sensors.requestTemperatures();
lcd.init();
lcd.clear();         
lcd.backlight();      // Make sure backlight is on
lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
lcd.print("Cooling Down");
lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
lcd.print("Temperature: ");
lcd.setCursor(12,1);   //Move cursor to character 12 on line 1
lcd.print(((sensors.getTempCByIndex(0) + sensors.getTempCByIndex(1))/2));    
digitalWrite(heater, HIGH);  // Set Pin to HIGH to turn Relay OFF, Stop Heating
digitalWrite(cooler, HIGH);  // Set Pin to HIGH to turn Relay ON for permanent cool down
Serial.println("**********************************************");
Serial.println("   HOT");
Serial.println("   Cooling Down");
Serial.println("   Completed");
while(true);


}
