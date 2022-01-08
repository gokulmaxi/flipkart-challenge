#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "motorGlobal.h"
#include "DFRobot_AS7341.h"
#include <math.h>
#include <Wire.h>

int wifiled = 5;

Servo servo;

DFRobot_AS7341 as7341;
// color data

 int red_array[] = {9, 21, 23, 13, 15, 45, 152, 152};

 int green_array[] = {17, 20, 21, 80, 141, 33, 21, 29};

 int dark_blue[] = {8, 21, 30, 14, 4, 20, 32, 13};

 int yellow[] = {18, 10, 35, 60, 184, 251, 295, 207};

 int violet[] = {27, 88, 141, 62, 70, 70, 91, 101};

 int brown[] = {16, 30, 59, 23, 40, 69, 100, 85};

 int white[] = {49, 222, 416, 388, 478, 474, 535, 380};

 int pink[] = {62, 149, 232, 139, 148, 311, 521, 377};

 int orange[] = {36, 16, 26, 31, 41, 170, 365, 276};

// networking
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int value = 0;
const char *device_name = "ESP32";
const char *ssid = "IA Lab";
const char *password = "rosdeveloper";
const char *mqtt_server = "192.168.0.163";

//util functions
bool eq(int n, int *x, int *y, int tolerance)
{
    const int absTolerance = std::abs(tolerance);
    for (unsigned int i = 0; i < n; ++i)
    {
        if (std::abs(x[i] - y[i]) > absTolerance)
        {
            return false;
        }
    }
    return true;
}

//set up ledc
void setupLedc()
{
    ledcSetup(ledChannel, freq, resolution);
    ledcSetup(ledChannel1, freq, resolution);
    ledcSetup(ledChannel2, freq, resolution);
    ledcSetup(ledChannel3, freq, resolution);
    ledcSetup(ledChannel4, freq, resolution);
    ledcSetup(ledChannel5, freq, resolution);
    ledcSetup(ledChannel6, freq, resolution);
    ledcSetup(ledChannel7, freq, resolution);
    ledcAttachPin(motor4Pin1, ledChannel5); // Motor 4 D26
    ledcAttachPin(motor4Pin2, ledChannel4); //Motor 4 D27

    ledcAttachPin(motor2Pin1, ledChannel7); // Motor 2 D32
    ledcAttachPin(motor2Pin2, ledChannel6); // Motor 2 D33

    ledcAttachPin(motor1Pin1, ledChannel3); // Motor 1 D18
    ledcAttachPin(motor1Pin2, ledChannel2); // Motor 1 D19

    ledcAttachPin(motor3Pin1, ledChannel1); // Motor 3 D12
    ledcAttachPin(motor3Pin2, ledChannel);  // Motor 3 D13

    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);

    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);

    pinMode(motor3Pin1, OUTPUT);
    pinMode(motor3Pin2, OUTPUT);

    pinMode(motor4Pin1, OUTPUT);
    pinMode(motor4Pin2, OUTPUT);

    pinMode(wifiled, OUTPUT);
}

void setup_wifi()
{
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    // int attempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(wifiled , HIGH);
        delay(1000);
        digitalWrite(wifiled,LOW);
        delay(1000);
        
        Serial.print(".");
    }

    digitalWrite(wifiled , HIGH);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(mqtt_server);
}


void reconnect()
{
    while (!client.connected())
    {

        Serial.print("Attempting MQTT connection...");
        if (client.connect(device_name))
        {
            Serial.println("connected");
            client.subscribe("flipkart/bot3");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}