#define RELAY_PIN D8

void setupRelay()
{

    pinMode(RELAY_PIN, OUTPUT);
}

void relayState(uint8_t s)
{
    NotifyElementPowerStatus(s);
    digitalWrite(RELAY_PIN, s);
}