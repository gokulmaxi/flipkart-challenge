
#include <WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <math.h>
#include <ESP32Servo.h>
//#include <Servo.h>
//Servo Motor Pin attach
Servo servo;
int serv = 0;
int rot = 0;
float mapPwm(float x, float out_min, float out_max)
{
  return x * (out_max - out_min) + out_min;
}

//Pin Allocation Driver Driver 1
int Driver1_A_PWM = 16;
int Driver1_A_FORW = 17;
int Driver1_A_BACK = 18;
int Driver1_B_PWM = 19;
int Driver1_B_FORW = 21;
int Driver1_B_BACK = 22;

//Pin Allocation Driver Driver 2
int Driver2_A_PWM = 23;
int Driver2_A_FORW = 25;
int Driver2_A_BACK = 26;
int Driver2_B_PWM = 27;
int Driver2_B_FORW = 32;
int Driver2_B_BACK = 33;

//PinMode Function
void setupPins() {
  pinMode(Driver1_A_PWM, OUTPUT);
  pinMode(Driver1_A_FORW, OUTPUT);
  pinMode(Driver1_A_BACK, OUTPUT);
  pinMode(Driver1_B_PWM, OUTPUT);
  pinMode(Driver1_B_FORW, OUTPUT);
  pinMode(Driver1_B_BACK, OUTPUT);
  pinMode(Driver2_A_PWM, OUTPUT);
  pinMode(Driver2_A_FORW, OUTPUT);
  pinMode(Driver2_A_BACK, OUTPUT);
  pinMode(Driver2_B_PWM, OUTPUT);
  pinMode(Driver2_B_FORW, OUTPUT);
  pinMode(Driver2_B_BACK, OUTPUT);
  servo.attach(15); //D4
  servo.write(0);
}

//Stop Function
void stops()
{
  //Driver Driver 1
  digitalWrite(Driver1_A_FORW, 0);
  digitalWrite(Driver1_A_BACK, 0);
  digitalWrite(Driver1_B_FORW, 0);
  digitalWrite(Driver1_B_BACK, 0);
  analogWrite(Driver1_A_PWM, 0);
  analogWrite(Driver1_B_PWM, 0);
  //Driver Driver 2
  digitalWrite(Driver2_A_FORW, 0);
  digitalWrite(Driver2_A_BACK, 0);
  digitalWrite(Driver2_B_FORW, 0);
  digitalWrite(Driver2_B_BACK, 0);
  analogWrite(Driver2_A_PWM, 0);
  analogWrite(Driver2_B_PWM, 0);
}


// WiFi
const char *ssid = "R&D LABS"; // Enter your WiFi name
const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "10.10.174.148";
const char *topic = "flipkart/bot4";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(topic);
  setupPins();
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  int i = 0;
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  StaticJsonDocument <256> doc;
  deserializeJson(doc, payload);
  float x = doc["data"][0];
  float z = doc["data"][1];
  serv = doc["data"][2];
  rot = doc["data"][3];
  Serial.println("x = ");
  Serial.println(x);
  Serial.println("z = ");
  Serial.println(z);
  Serial.print("serv= ");
  Serial.println(serv);
  Serial.print("rot =");
  Serial.println(rot);
  Serial.println("-----------------------");


  //Capping the incoming 'x' and 'z' co-ordinates at [-1 to 1]

  x = max(min(x, 1.0f), -1.0f);
  z = max(min(z, 1.0f), -1.0f);
  //Intensity calculation
  float l = (x - z) / 2;
  float r = (x + z) / 2;
  Serial.println(l);
  Serial.println(r);

  uint16_t AlPwm = mapPwm(fabs(l), 50, 250);
  uint16_t ArPwm = mapPwm(fabs(l), 50, 250);
  uint16_t BlPwm = mapPwm(fabs(r), 50, 250);
  uint16_t BrPwm = mapPwm(fabs(r), 50, 250);
  //Forward (+, +)
  //Backward (-, -)
  //Left (-, +)
  //Right (+, -)
  //Driver Driver 1
  analogWrite(Driver1_A_PWM, AlPwm+15);
//  serial.println(AlPwm);
  digitalWrite(Driver1_A_FORW, l > 0);
  digitalWrite(Driver1_A_BACK, l < 0);
  analogWrite(Driver1_B_PWM, ArPwm+15);
  digitalWrite(Driver1_B_FORW, l > 0);
  digitalWrite(Driver1_B_BACK, l < 0);

  //Driver Driver 2
  analogWrite(Driver2_A_PWM, BlPwm);
  digitalWrite(Driver2_A_FORW, r > 0);
  digitalWrite(Driver2_A_BACK, r < 0);
  analogWrite(Driver2_B_PWM, BrPwm);
  digitalWrite(Driver2_B_FORW, r > 0);
  digitalWrite(Driver2_B_BACK, r < 0);
  //Stop
  if (rot == 3) {
    stops();
  }
  //Left
  if (rot == 1) {

    digitalWrite(Driver1_A_FORW, LOW);
    digitalWrite(Driver1_A_BACK, HIGH);
    digitalWrite(Driver1_B_FORW, LOW);
    digitalWrite(Driver1_B_BACK, HIGH);
    
    analogWrite(Driver1_A_PWM, 80);
    analogWrite(Driver1_B_PWM, 80);
    //Driver Driver 2
    digitalWrite(Driver2_A_FORW, HIGH);
    digitalWrite(Driver2_A_BACK, LOW);
    digitalWrite(Driver2_B_FORW, HIGH);
    digitalWrite(Driver2_B_BACK, LOW);
    analogWrite(Driver2_A_PWM, 80);
    analogWrite(Driver2_B_PWM, 80);

    Serial.println("left");
  }
  //Right
  if (rot == 2) {
    digitalWrite(Driver1_A_FORW, HIGH);
    digitalWrite(Driver1_A_BACK, LOW);
    digitalWrite(Driver1_B_FORW, HIGH);
    digitalWrite(Driver1_B_BACK, LOW);
    analogWrite(Driver1_A_PWM, 80);
    analogWrite(Driver1_B_PWM, 80);
    //Driver Driver 2
    digitalWrite(Driver2_A_FORW, LOW);
    digitalWrite(Driver2_A_BACK, HIGH);
    digitalWrite(Driver2_B_FORW, LOW);
    digitalWrite(Driver2_B_BACK, HIGH);
    analogWrite(Driver2_A_PWM, 80);
    analogWrite(Driver2_B_PWM, 80);

    Serial.println("right");

  }
  if (serv == 1) {
    servo.write(0);
    delay(500);
    servo.write(150);
    delay(500);
    servo.write(0);
    delay(500);
  }
}

void loop() {
  client.loop();
}
