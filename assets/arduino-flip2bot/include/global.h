#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "motorGlobal.h"
#include <math.h>
#include <Wire.h>
#include <string.h>
// #include <Adafruit_TCS34725.h>
// #include <SPI.h>

//Todo Change Device name, topic, destination topic according to bot number.
char device_name[] = "ESP32-bot3";
char device_topic[] = "flipkart/bot3";
// char destination_topic[] = "flipkart/color2";

void callback(char *topic, byte *message, unsigned int length);

int wifiled = 5;

const int angularPwm = 180;
const int linearPwm = 200;
const int lineargoalPwm = 180;

Servo servo;

// //Retriving Color from RGB data from TCS3475 RGB sensor

// int red[] = {127, 85, 85};

// int green[] = {36, 100, 77};

// int dark_blue[] = {31, 74, 121};

// int yellow[] = {91, 85, 50};

// int violet[] = {58, 80, 102};

// int brown[] = {85, 85, 56};

// int white[] = {73, 94, 59};

// int pink[] = {97, 59, 89};

// int orange[] = {191, 31, 31};

// Networking Credentials

WiFiClient espClient;
PubSubClient client(espClient);
const char *ssid = "IA LAB";
const char *password = "rosdeveloper";
const char *mqtt_server = "192.168.0.116";

//util compare function
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

//setup Ledc
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
//Setup WiFi properties
void setup_wifi()
{
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(wifiled, HIGH);
        delay(1000);
        digitalWrite(wifiled, LOW);
        delay(1000);

        Serial.print(".");
    }

    digitalWrite(wifiled, HIGH);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(mqtt_server);
}
//Reconnect to WiFi
void reconnect()
{
    while (!client.connected())
    {

        Serial.print("Attempting MQTT connection...");
        if (client.connect(device_name))
        {
            Serial.println("connected");
            client.subscribe(device_topic);
            Serial.println(device_topic);
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

//Sorting packages with respect to colors
// int color()
// {   

//     Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//     float r, g, b;

//     tcs.getRGB(&r, &g, &b);

//     delay(100);

//     Serial.println(int(r));
//     Serial.println(int(g));
//     Serial.println(int(b));
//     Serial.println("-------------------------------------");

//     int currentRGB[] = {int(r), int(g), int(b)};

//     if (eq(2, red, currentRGB, 10))
//     {
//         return 1; // PUNE
//     }
//     else if (eq(2, green, currentRGB, 20))
//     {
//         return 2; // AMEMDABAD
//     }
//     else if (eq(2, dark_blue, currentRGB, 10))
//     {
//         return 3; // JAIPUR
//     }
//     else if (eq(2, yellow, currentRGB, 10))
//     {
//         return 4; // CHENNAI
//     }
//     else if (eq(2, violet, currentRGB, 10))
//     {
//         return 5; // BENGALURU
//     }
//     else if (eq(2, brown, currentRGB, 10))
//     {
//         return 6; // HYDERABAD
//     }
//     else if (eq(2, white, currentRGB, 10))
//     {
//         return 7; // MUMBAI
//     }
//     else if (eq(2, pink, currentRGB, 10))
//     {
//         return 8; // DELHI
//     }
//     else if (eq(2, orange, currentRGB, 10))
//     {
//         return 9; //KOLKATA
//     }
//     else
//     {
//         return 0;
//     }
// }