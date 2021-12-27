#include "Arduino.h"
#include "global.h"

bool colorpub = true;

String colorvalue;
char colors[50];

void callback(char *topic, byte *message, unsigned int length);

const int angularPwm = 180;
const int linearPwm = 200;

void setup()
{

  setupLedc();

  while (as7341.begin() != 0)
  {
    Serial.println("IIC init failed, please check if the wire connection is correct");
    delay(1000);
  }
  as7341.enableLed(true);

  as7341.controlLed(1);

  servo.attach(15); //D4
  servo.write(90);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char *topic, byte *message, unsigned int length)
{

  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    //    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "flipkart/bot1")
  {
    //    Serial.print("Changing output to ");
    if (messageTemp == "message")
    {
      client.publish("flipkart/bot1", "Message Received at ESP32");

      //      Serial.println("on");
    }
    else if (messageTemp == "off")
    {
      Serial.println("off");
    }
  }
  Serial.println();
  StaticJsonDocument<256> doc;
  deserializeJson(doc, message);
  int angular = doc["angular"];
  int linear_x = doc["linear_x"];
  int linear_y = doc["linear_y"];
  int servo_data = doc["servo"];

  Serial.println(angular);
  Serial.println(linear_x);
  Serial.println(linear_y);
  // Angular left

  if (angular == 1)
  {
    if (abs(linear_x) == 1)
    {
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel1, angularPwm);

      ledcWrite(ledChannel2, angularPwm);
      ledcWrite(ledChannel3, 0);
      delay(50);
      allHalt();
    }
    if (abs(linear_y) == 1)
    {
      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, angularPwm);

      ledcWrite(ledChannel6, angularPwm);
      ledcWrite(ledChannel7, 0);
      delay(50);
      allHalt();
    }
  }

  if (angular == -1)
  {
    if (abs(linear_x) == 1)
    {
      ledcWrite(ledChannel, angularPwm);
      ledcWrite(ledChannel1, 0);

      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, angularPwm);
      delay(50);
      allHalt();
    }
    if (abs(linear_y) == 1)
    {
      ledcWrite(ledChannel4, angularPwm);
      ledcWrite(ledChannel5, 0);

      ledcWrite(ledChannel6, 0);
      ledcWrite(ledChannel7, angularPwm);
      delay(50);
      allHalt();
    }
  }

  if (angular == 0)
  {
    if (linear_x == 0 && linear_y == 0)
    {
      allHalt();
    }

    //forward

    if (linear_x == 1)
    {
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel1, 0);

      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, 0);

      ledcWrite(ledChannel4, linearPwm);
      ledcWrite(ledChannel5, 0);

      ledcWrite(ledChannel6, linearPwm);
      ledcWrite(ledChannel7, 0);
    }

    //backward
    if (linear_x == -1)
    {
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel1, 0);

      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, 0);

      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, linearPwm);

      ledcWrite(ledChannel6, 0);
      ledcWrite(ledChannel7, linearPwm);
    }

    if (linear_y == 1)
    {
      ledcWrite(ledChannel, linearPwm);
      ledcWrite(ledChannel1, 0);

      ledcWrite(ledChannel2, linearPwm);
      ledcWrite(ledChannel3, 0);

      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, 0);

      ledcWrite(ledChannel6, 0);
      ledcWrite(ledChannel7, 0);
    }

    if (linear_y == -1)
    {
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel1, linearPwm);

      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, linearPwm);

      ledcWrite(ledChannel4, 0);
      ledcWrite(ledChannel5, 0);

      ledcWrite(ledChannel6, 0);
      ledcWrite(ledChannel7, 0);
    }
  }

  if (servo_data == 1)
  {
    servo.write(0);
    // Serial.println("its zero");
    delay(1000);
    servo.write(90);
    // Serial.println("its ninety");
    delay(1000);
  }
  if (servo_data == -1)
  {
    servo.write(180);
    // Serial.print("its 150");
    delay(1000);
    servo.write(90);
    // Serial.println("its ninety");
    delay(1000);
  }
}

int color()
{
  DFRobot_AS7341::sModeOneData_t data1;
  DFRobot_AS7341::sModeTwoData_t data2;

  //Start spectrum measurement
  //Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
  as7341.startMeasure(as7341.eF1F4ClearNIR);
  //Read the value of sensor data channel 0~5, under eF1F4ClearNIR
  data1 = as7341.readSpectralDataOne();

  as7341.startMeasure(as7341.eF5F8ClearNIR);
  //Read the value of sensor data channel 0~5, under eF5F8ClearNIR
  data2 = as7341.readSpectralDataTwo();

  int curr[] = {data1.ADF1, data1.ADF2, data1.ADF3, data1.ADF4, data2.ADF5, data2.ADF6, data2.ADF7, data2.ADF8};

  if (eq(8, red_array, curr, 50))
  {
    // Serial.println("RED ");
    // client.publish("flipkart/color1", "1");
    int colorvalue = 1;
    return colorvalue;
    
  }
  else if (eq(8, green_array, curr, 50))
  {
    // Serial.println("GREEN");
    // client.publish("flipkart/color1", "2");
        int colorvalue = 2;
    return colorvalue;
  }
  else if (eq(8, dark_blue, curr, 20))
  {
    // Serial.println("DARKBLUE");
    // client.publish("flipkart/color1", "3");
        int colorvalue = 3;
    return colorvalue;
  }
  else if (eq(8, yellow, curr, 50))
  {
    // Serial.println("YELLOW");
    // client.publish("flipkart/color1", "4");
        int colorvalue = 4;
    return colorvalue;
  }
  else if (eq(8, violet, curr, 90))
  {
    // Serial.println("VIOLET");
    // client.publish("flipkart/color1", "5");
        int colorvalue = 5;
    return colorvalue;
  }
  else if (eq(8, brown, curr, 50))
  {
    // Serial.println("BROWN");
    // client.publish("flipkart/color1", "6");
        int colorvalue = 6;
    return colorvalue;
  }
  else if (eq(8, white, curr, 100))
  {
    Serial.println("WHITE");
    // client.publish("flipkart/color1", "7");
        int colorvalue = 7;
    return colorvalue;
  }
  else if (eq(8, pink, curr, 50))
  {
    // Serial.println("PINK");
    // client.publish("flipkart/color1", "8");
        int colorvalue = 8;
    return colorvalue;
  }
  else if (eq(8, orange, curr, 70))
  {
    // Serial.println("ORANGE");
    // client.publish("flipkart/color1", "9");
        int colorvalue = 9;
    return colorvalue;
  }

  else
  {
    Serial.println("No Cube detected");
    return 0;
  }


}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  // color();

  DFRobot_AS7341::sModeOneData_t data1;
  DFRobot_AS7341::sModeTwoData_t data2;

  data1 = as7341.readSpectralDataOne();
  int nearIR = data1.ADNIR;

  if (nearIR >= 168 && nearIR <=190)
  {

    if (colorpub)
    {

      colorvalue = color();

      Serial.println(colorvalue);

      colorvalue.toCharArray(colors , colorvalue.length() + 1);

      client.publish("flipkart/color1", colors);

      colorpub = false;
    }
  }
  else
  {
    colorpub = true;
  }
}