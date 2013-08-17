/*
  FishTankApp.ino - Script to communicate with hardware device on the fish tank.
  Created by Josh Villbrandt (http://javconcepts.com/), August 16, 2013.
  Released into the public domain.
*/
// OneWire Library: http://www.pjrc.com/teensy/td_libs_OneWire.html
// DallasTemperature Library: http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
// using a TMP36 for the ambient air sensor: https://www.sparkfun.com/products/10988
// using a waterproof DS18B20 for the tank sensor: https://www.sparkfun.com/products/11050
// serial interface copied from: https://gist.github.com/biomood/955869

#include <OneWire.h>
#include <DallasTemperature.h>
#define DEBUG false
#define COUNTS_TO_VOLTS (5.0/1023.0)
char recMsg = '0';

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#define ONE_WIRE_BUS_PIN 2
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress tankAddress;

void setup(void)
{
  // start serial port
  Serial.begin(57600);
  if(DEBUG) Serial.println("Dallas Temperature IC Control Library Demo");

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
  if(DEBUG) {
    Serial.print("Device 0 Address: ");
    printAddress(tankAddress);
    Serial.println();
  }

  // set the resolution to 12 bit
  sensors.setResolution(tankAddress, 12);
  if(DEBUG) {
    Serial.print("Device 0 Resolution: ");
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
  
  // tank temp sensor
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tTankF = sensors.getTempF(tankAddress);
  
  // photo diode for tank light
  int lightRaw = analogRead(A0);
  // light off is about 20, light on is about 36
  boolean light = lightRaw > 28;
  
  // ambient temp sensor
  float tAmbientRaw = analogRead(A1)*COUNTS_TO_VOLTS;
  float tAmbientC = 100.0 * tAmbientRaw - 50.0;
  //float tAmbientF = tAmbientC * 9.0 / 5.0 + 32.0;
  float tAmbientF = DallasTemperature::toFahrenheit(tAmbientC);
  
  // It responds almost immediately. Let's print out the data
  if(DEBUG) {
    Serial.print("Light: ");
    //Serial.print(lightRaw);
    if(light) Serial.print("on ");
    else Serial.print("off");
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
