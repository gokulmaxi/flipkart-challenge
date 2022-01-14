//Motor 1
int motor1Pin1 = 18;
int motor1Pin2 = 19;

//Motor 2
int motor2Pin1 = 32;
int motor2Pin2 = 33;

//Motor 3
int motor3Pin1 = 12;
int motor3Pin2 = 13;

//Motor 4
int motor4Pin1 = 26;
int motor4Pin2 = 27;


const int freq = 5000;

const int ledChannel = 8;
const int ledChannel1 = 9;
const int ledChannel2 = 10;
const int ledChannel3 = 11;
const int ledChannel4 = 12;
const int ledChannel5 = 13;
const int ledChannel6 = 14;
const int ledChannel7 = 15;

// PWM resolution

const int resolution = 8;
//Stop
void allHalt()
{
    ledcWrite(ledChannel, 0);
    ledcWrite(ledChannel1, 0);

    ledcWrite(ledChannel2, 0);
    ledcWrite(ledChannel3, 0);

    ledcWrite(ledChannel4, 0);
    ledcWrite(ledChannel5, 0);

    ledcWrite(ledChannel6, 0);
    ledcWrite(ledChannel7, 0);
}