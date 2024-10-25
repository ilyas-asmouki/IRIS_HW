#include <Wire.h>

// i2c slave addresses
#define FR_ADDRESS 0x10
#define FL_ADDRESS 0x11
#define RR_ADDRESS 0x12
#define RL_ADDRESS 0x13

// max length for incoming serial data
#define MAX_INPUT_LENGTH 50

#define I2C_CLOCK_FREQ 400000
#define BAUD_RATE 115200

int timestamp = 0;

char input_buffer[MAX_INPUT_LENGTH];
int buffer_idx = 0;

void setup() {
  Wire.begin();
  Wire.setClock(I2C_CLOCK_FREQ);
  Serial.begin(BAUD_RATE);
}

void loop() {
  while (Serial.available() > 0) {
    char in_char = (char) Serial.read();
    if (in_char == '\n' || buffer_idx >= MAX_INPUT_LENGTH - 1) {
      input_buffer[buffer_idx] = '\0';
      process_command();
      buffer_idx = 0;
    } else {
      input_buffer[buffer_idx++] = in_char;
    }
  }
}

void process_command() {
  int fr_velocity = 0;
  int fl_velocity = 0;
  int rr_velocity = 0;
  int rl_velocity = 0;

  char *token = strtok(input_buffer, " ");
  
  while (token != NULL) {
    char *motor = token;
    token = strtok(NULL, " ");
    if (token == NULL) break;

    int velocity = atoi(token);

    if (strcmp(motor, "fr") == 0) {
      fr_velocity = velocity;
    } else if (strcmp(motor, "fl") == 0) {
      fl_velocity = velocity;
    } else if (strcmp(motor, "rr") == 0) {
      rr_velocity = velocity;
    } else if (strcmp(motor, "rl") == 0) {
      rl_velocity = velocity;
    }

    token = strtok(NULL, " ");
  }

  Serial.println("=======================");
  Serial.println(timestamp++);
  send_to_motor(fr_velocity, FR_ADDRESS);
  send_to_motor(fl_velocity, FL_ADDRESS);
  send_to_motor(rr_velocity, RR_ADDRESS);
  send_to_motor(rl_velocity, RL_ADDRESS);
}

void send_to_motor(int velocity, int slave_address) {
  if (slave_address != 0) {
    char direction = (velocity >= 0) ? 'f' : 'b';
    velocity = abs(velocity);

    Wire.beginTransmission(slave_address);
    Wire.write(direction);
    Wire.write(velocity);
    uint8_t error_code = Wire.endTransmission();

    // debug info
    Serial.print("Motor command sent to 0x");
    Serial.print(slave_address, HEX);
    Serial.print(": Direction = ");
    Serial.print(direction);
    Serial.print(", Velocity = ");
    Serial.print(velocity);
    Serial.print(", Error Code: ");
    Serial.println(error_code);
  }
}
