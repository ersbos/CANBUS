

// Pins
const int Engine_Vcc = 11;
const int Engine_Gnd = 12;
const int on_off_pin = 3;
const int speed_control_read_pin = A0;
const int speed_control_write_pin = 5;

// Functions
void on_off_function();
void set_motor_direction(int);
void speed_control(int); 
void emergency(String);

// Initial values
int on_off = 0;
int direction = 0;
int speed = 0;

void setup() {
  pinMode(Engine_Vcc, OUTPUT);
  pinMode(Engine_Gnd, OUTPUT);
  pinMode(on_off_pin, INPUT);
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  if (Serial.available() > 0) { 
    String inputString = Serial.readStringUntil('\n'); 
    if (inputString == "STOP") {
      emergency(inputString);
    } else {
      int direction = inputString.toInt(); 
      set_motor_direction(direction);
    }
  }

  int speed = analogRead(speed_control_read_pin); 
  speed = map(speed, 0, 1023, 0, 255); 
  
  on_off_function();
  speed_control(speed);
}

void on_off_function() {
  int on_off = digitalRead(on_off_pin);
  if (on_off == HIGH) {
    digitalWrite(Engine_Vcc, HIGH); 
  } else {
    digitalWrite(Engine_Vcc, LOW);  
  }
}

void set_motor_direction(int direction) {
  if (direction == 1) {
    digitalWrite(Engine_Vcc, HIGH); 
    digitalWrite(Engine_Gnd, LOW);  
  } else if (direction == 0) {
    digitalWrite(Engine_Vcc, LOW);  
    digitalWrite(Engine_Gnd, HIGH); 
  } else {
    Serial.println("Invalid direction! Please enter only 0 or 1.");
  }
}

void speed_control(int speed) {
  analogWrite(speed_control_write_pin, speed);
}

void emergency(String command) {
  digitalWrite(Engine_Vcc, HIGH); 
  digitalWrite(Engine_Gnd, HIGH); 
}
