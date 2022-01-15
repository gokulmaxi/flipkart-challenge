#include "Arduino.h"
#include "global.h"

void setup()
{

  setupLedc();

  Serial.begin(115200);

  servo.attach(15); //D4
  servo.write(90);

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void callback(char *topic, byte *message, unsigned int length)
{
  StaticJsonDocument<256> doc;
  deserializeJson(doc, message);
  int angular = doc["angular"];
  int linear_x = doc["linear_x"];
  int linear_y = doc["linear_y"];
  int servo_data = doc["servo"];
  int cube = doc["colorRequest"];

  if (cube == 1)
  {
    int colordata = color();
    if(colordata != 0)
    {
    String returnString = String(colordata);
    char destination[10];
    returnString.toCharArray(destination, 10); 
    client.publish(destination_topic, destination);  
    }
  }
  
  // Angular left

  if (angular == 1)
  {
    if (abs(linear_y) == 1)
    {
      ledcWrite(ledChannel, angularPwm);
      ledcWrite(ledChannel1, 0);

      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, angularPwm);
      delay(50);
      allHalt();
    }
    if (abs(linear_x) == 1)
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
    if (abs(linear_y) == 1)
    {

      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel1, angularPwm);

      ledcWrite(ledChannel2, angularPwm);
      ledcWrite(ledChannel3, 0);
      delay(50);
      allHalt();
    }
    if (abs(linear_x) == 1)
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

    if (linear_y == 1)
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
    if (linear_y == -1)
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

    if (linear_x == -1)
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

    if (linear_x == 1)
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
    // Serial.println("its 0 degrees");
    delay(1000);
    servo.write(90);
    // Serial.println("its 90 degrees");
    delay(1000);
  }
  if (servo_data == -1)
  {
    servo.write(180);
    // Serial.print("its 150 degrees");
    delay(1000);
    servo.write(90);
    // Serial.println("its 90 degrees");
    delay(1000);
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}