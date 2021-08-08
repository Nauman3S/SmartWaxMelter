void setupNotificationLEDs()
{
    pinMode(D4, OUTPUT);
    pinMode(D3, OUTPUT);
}

void NotifyWiFiConnected(bool v)
{
    if (v == false)
    {
        digitalWrite(D4, LOW);
    }
    else
    {
        digitalWrite(D4, HIGH);
    }
}

void NotifyElementPowerStatus(bool v)
{
    if (v == false)
    {
        digitalWrite(D3, LOW);
    }
    else
    {
        digitalWrite(D3, HIGH);
    }
}