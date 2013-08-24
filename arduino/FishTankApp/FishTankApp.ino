/*
  FishTankApp.ino - Script to communicate with hardware device on the fish tank.
  Created by Josh Villbrandt (http://javconcepts.com/), August 16, 2013.
*/

// OneWire Library: http://www.pjrc.com/teensy/td_libs_OneWire.html
// DallasTemperature Library: http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
// using a DS18B20 for the ambient air sensor: https://www.sparkfun.com/products/245
// using a waterproof DS18B20 for the tank sensor: https://www.sparkfun.com/products/11050
// serial communication scheme copied from: https://gist.github.com/biomood/955869

#include <OneWire.h>
#include <DallasTemperature.h>
#define DEBUG false
#define COUNTS_TO_VOLTS (5.0/1023.0)
char recMsg = '0';

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#define ONE_WIRE_BUS_PIN 2
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress ambientAddress, tankAddress;

void setup(void)
{
  // start serial port
  Serial.begin(57600);

  // locate devices on the bus
  if(DEBUG) Serial.print("Locating devices...");
  sensors.begin();
  if(DEBUG) {
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
  }

  // report parasite power requirements
  if(DEBUG) {
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
  }

  // grab address
  if (!sensors.getAddress(tankAddress, 0)) {
    if(DEBUG) Serial.println("Unable to find address for Device 0");
  }
  if (!sensors.getAddress(ambientAddress, 1)) {
    if(DEBUG) Serial.println("Unable to find address for Device 1");
  }
  if(DEBUG) {
    Serial.print("Addresses: ");
    printAddress(tankAddress);
    Serial.print(", ");
    printAddress(ambientAddress);
    Serial.println();
  }

  // set the resolution to 12 bit
  sensors.setResolution(ambientAddress, 12);
  sensors.setResolution(tankAddress, 12);
  if(DEBUG) {
    Serial.print("Resolutions: ");
    Serial.print(sensors.getResolution(ambientAddress), DEC); 
    Serial.print(", ");
    Serial.print(sensors.getResolution(tankAddress), DEC); 
    Serial.println();
  }
}

void loop(void)
{
  // wait for data request
  if(!DEBUG) {
    while(Serial.available() > 0) {
      recMsg = Serial.read();
    }
  }
  
  // temp sensors
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tAmbientF = sensors.getTempF(ambientAddress);
  float tTankF = sensors.getTempF(tankAddress);
  
  // photo diode for tank light
  int lightRaw = analogRead(A0);
  
  // It responds almost immediately. Let's print out the data
  if(DEBUG) {
    Serial.print("Light: ");
    Serial.print(lightRaw);
    Serial.print("   Ambient: ");
    Serial.print(tAmbientF);
    Serial.print("F   Tank: ");
    Serial.print(tTankF);
    Serial.println("F");
  }
  else if(recMsg!='0') {
    Serial.print(lightRaw);
    Serial.print(',');
    Serial.print(tAmbientF);
    Serial.print(',');
    Serial.print(tTankF);
  }
  
  // force us to wait for serial input again
  recMsg = '0';
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
