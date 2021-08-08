
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SoftwareStack.h"
#include "notificationLEDS.h"
#include "RelayController.h"
#include "ds18b20Lib.h"
#include "LEDStrip.h"

// Update these with values suitable for your network.

const char *ssid = "WiFiName";
const char *password = "WiFiPassword";
const char *mqtt_server = "broker.hivemq.com";

String topicTempV = "";
String topicRelay = "";
String topicLEDStrip = "";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
SoftwareStack ss;

String getMacAddress()
{
    String macAddressV = String(WiFi.macAddress());

    macAddressV = String(macAddressV[0]) + String(macAddressV[1]) + String(macAddressV[3]) + String(macAddressV[4]) +
                  String(macAddressV[6]) + String(macAddressV[7]) + String(macAddressV[9]) + String(macAddressV[10]) +
                  String(macAddressV[12]) + String(macAddressV[13]) + String(macAddressV[15]) + String(macAddressV[16]);
    //Serial.print("This Device MAC Address: ");
    //Serial.println(macAddressV);
    return macAddressV;
}

void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String payloadV = "";
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        payloadV = payloadV + String((char)payload[i]);
    }
    Serial.println();
    if (String(topic) == topicLEDStrip)
    {
        String rC = ss.StringSeparator(payloadV, ',', 0);
        String gC = ss.StringSeparator(payloadV, ',', 1);
        String bC = ss.StringSeparator(payloadV, ',', 2);
        String brightnessC = ss.StringSeparator(payloadV, ',', 3);
        ledColor(rC.toInt(), gC.toInt(), bC.toInt(), brightnessC.toInt());
    }
    if (String(topic) == topicRelay)
    {
        if (payloadV == String("on"))
        {
            relayState(1);
        }
        if (payloadV == String("off"))
        {
            relayState(0);
        }
    }
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
                                        // but actually the LED is on; this is because
                                        // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
    }
}

void mqttPublish(String path, String msg)
{
    //String path = String("channels/") + channelId + String("/publish/") + apiKey;
    client.publish(path.c_str(), msg.c_str());
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            NotifyWiFiConnected(true);
            // Once connected, publish an announcement...
            //client.publish("outTopic", "hello world");
            // ... and resubscribe
            //client.subscribe("inTopic");
            client.subscribe(topicRelay.c_str());
            client.subscribe(topicLEDStrip.c_str());
        }
        else
        {
            Serial.print("failed, rc=");
            NotifyWiFiConnected(false);
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    setupRelay();
    setupDS18B20();
    setupNotificationLEDs();
    setupLEDStrip();

    Serial.begin(115200);
    topicTempV = String("SmartWaxMelter") + getMacAddress() + String("temperature");
    topicRelay = String("SmartWaxMelter") + getMacAddress() + String("relayStatus");
    topicLEDStrip = String("SmartWaxMelter") + getMacAddress() + String("ledStrip");
    setup_wifi();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 1000)
    {
        lastMsg = now;
        mqttPublish(topicTempV, String(getTemperature()));
    }
}