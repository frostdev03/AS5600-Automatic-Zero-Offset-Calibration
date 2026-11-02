#include <Wire.h>

#define AS5600_ADDRESS 0x36  // I2C default address

uint16_t zero_offset = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);  // SDA = 21, SCL = 22
  Serial.println("Checking AS5600...");

  delay(500);  // Wait
  zero_offset = readAS5600RawAngle();  // Starting pos in zero
  Serial.print("Starting offset saved: ");
  Serial.println(zero_offset);
}

void loop() {
  uint16_t rawAngle = readAS5600RawAngle();

  int16_t corrected = rawAngle - zero_offset;
  if (corrected < 0) corrected += 4096;

  float degAngle = corrected * 0.08789; // 360 / 4096

  Serial.print("Raw: ");
  Serial.print(rawAngle);
  Serial.print(" | Corrected: ");
  Serial.print(corrected);
  Serial.print(" | Degree: ");
  Serial.println(degAngle, 2);

  delay(500);
}

// Read raw_angle AS5600
uint16_t readAS5600RawAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(0x0C);  // Register MSB raw_angle
  Wire.endTransmission(false);

  Wire.requestFrom(AS5600_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    return ((msb << 8) | lsb);
  } else {
    Serial.println("Failed!");
    return 0;
  }
}
