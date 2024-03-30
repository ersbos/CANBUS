/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ir-remote-control
 */
#include <NewPing.h>
#include <DIYables_IRcontroller.h> // DIYables_IRcontroller library
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define IR_RECEIVER_PIN 19 // The ESP32 pin GPIO19 connected to IR controller

#define TRIGGER_PIN 5
#define ECHO_PIN 18
#define TRIGGER_PIN2 26
#define ECHO_PIN2 27

#define MAX_DISTANCE 300

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

Adafruit_MPU6050 mpu;

DIYables_IRcontroller_17 irController(IR_RECEIVER_PIN, 200); // debounce time is 200ms

void setup() {
  Serial.begin(9600);
  irController.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
Serial.println("MPU6050 Found!");

mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);



}
}

void loop() {
//mpu
sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);





//proximity
delay(150);
int uS = sonar.ping();
int uS2 =sonar2.ping();


if (uS2==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN, LOW);
delay(150);
pinMode(ECHO_PIN, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping2: ");
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
}
if (uS==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN, LOW);
delay(150);
pinMode(ECHO_PIN, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping: ");
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
}


//IR
  Key17 key = irController.getKey();
  if (key != Key17::NONE) {
    switch (key) {
      case Key17::KEY_1:
        Serial.println("1");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_2:
        Serial.println("2");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_3:
        Serial.println("3");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_4:
        Serial.println("4");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_5:
        Serial.println("5");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_6:
        Serial.println("6");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_7:
        Serial.println("7");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_8:
        Serial.println("8");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_9:
        Serial.println("9");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_STAR:
        Serial.println("*");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_0:
        Serial.println("0");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_SHARP:
        Serial.println("#");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_UP:
        Serial.println("UP");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_DOWN:
        Serial.println("DOWN");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_LEFT:
        Serial.println("LEFT");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_RIGHT:
        Serial.println("RIGHT");
        // TODO: YOUR CONTROL
        break;

      case Key17::KEY_OK :
        Serial.println("OK");
        // TODO: YOUR CONTROL
        break;

      default:
        Serial.println("WARNING: undefined key:");
        break;
    }
  }
}

