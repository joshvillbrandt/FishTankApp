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
#define DEBUG true

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
#define ONE_WIRE_BUS_PIN 2
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress tankAddress;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // grab address
  if (!sensors.getAddress(tankAddress, 0)) Serial.println("Unable to find address for Device 0");
  Serial.print("Device 0 Address: ");
  printAddress(tankAddress);
  Serial.println();

  // set the resolution to 12 bit
  sensors.setResolution(tankAddress, 12);
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(tankAddress), DEC); 
  Serial.println();
}

// function to print the temperature for a device
void printTemperature(float tempC)
{
  //Serial.print(tempC);
  //Serial.print("C, ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print("F");
}

void loop(void)
{
  // dallas sensor
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  // photo diode
  int lightRaw = analogRead(A0);
  // light off is about 20
  // light on is about 36
  boolean light = lightRaw > 28;
  
  // ambient temp sensor
  int tAmbientRaw = analogRead(A1);
  float tAmbientC = 100.0 * (tAmbientRaw/1023.0)*5.0 - 50.0;
  //float tAmbientF = tAmbientC * 9.0 / 5.0 + 32.0;
  // print out the value you read:
  //Serial.print(tAmbientRaw);
  //Serial.print("\t\t");
  //Serial.println(tAmbientF);
  //delay(10);        // delay in between reads for stability
  
  // It responds almost immediately. Let's print out the data
  if(DEBUG) {
    Serial.print("Light: ");
    //Serial.print(lightRaw);
    if(light) Serial.print("on ");
    else Serial.print("off");
    Serial.print("   Ambient: ");
    printTemperature(tAmbientC); // Use a simple function to print out the data
    Serial.print("   Tank: ");
    printTemperature(sensors.getTempC(tankAddress)); // Use a simple function to print out the data
    Serial.println("");
  }
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
