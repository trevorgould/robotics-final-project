#include <Sparki.h>

String inputString = "";

void setup() {
  Serial.begin(9600);
  sparki.servo(SERVO_CENTER);
}

void loop() {
  sparki.RGB(0, 0, 100);
  while (Serial.available()) {

    char inChar = (char) Serial.read();

    if (inChar == 'L') {

      sparki.servo(SERVO_LEFT);
      Serial.println("SERVO_LEFT");
      break;

    } else if (inChar == 'R') {

      sparki.servo(SERVO_RIGHT);
      Serial.println("SERVO_RIGHT");
      break;

    } else if (inChar == 'W') {

    sparki.moveForward(5);

    } else if (inChar == 'S') {

      sparki.moveBackward(5);

    } else {

      return;
    }
  }
}
