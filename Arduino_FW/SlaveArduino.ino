#include <Wire.h>

#define IN1 5
#define IN2 6

// i2c slave addresses 
#define FR_ADDRESS 0x10
#define FL_ADDRESS 0x11
#define RR_ADDRESS 0x12
#define RL_ADDRESS 0x13

#define SLAVE_ADDRESS FR_ADDRESS

char direction = '\0';
int velocity = 0;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receive_signal);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
}

void loop() {
  // no need to do anything in the loop, everything happens in the receive_signal function
  if (velocity != 0) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void receive_signal(int bytes) {
  if (bytes >= 2) {
    direction = Wire.read();  // read direction 'f' or 'b'
    velocity = Wire.read();   // read velocity (PWM value)

    // Control motor based on direction and velocity
    if (direction == 'f' and velocity != 0) {
      analogWrite(IN1, velocity);
      analogWrite(IN2, 0);
      Serial.println(velocity);
      delay(10);
    } else if (direction == 'b' and velocity != 0) {
      analogWrite(IN1, 0);
      analogWrite(IN2, velocity);
      Serial.println(velocity);
      delay(10);
    } else {
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
    }
  }
}
