#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS D2

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setupDS18B20(void)
{
    // start serial port
    Serial.begin(9600);
    Serial.println("Dallas Temperature IC Control Library Demo");

    // Start up the library
    sensors.begin();
}

float getTemperature(void)
{
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    // Serial.print(" Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
                                   // Serial.println("DONE");

    //Serial.print("Temperature for Device 1 is: ");
    //Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?
    return (float)sensors.getTempCByIndex(0);
    // You can have more than one IC on the same bus.
    // 0 refers to the first IC on the wire
}
