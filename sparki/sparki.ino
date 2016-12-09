#include <Sparki.h>
String inputString = "";
#define DRIVE 0
#define GRAB 1
#define ROTATE 3
#define FOLLOW 4
#define DROP 5
#define LOOP_TIME         100
#define AXLE_DIST         0.08586
#define RAD               0.025
#define VEL               0.0278551532
#define LOOP_TIME_O       100
#define THRESHOLD         700
#define VELOCITY          0.0278551532

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
      Serial.println("SERVO_LEFT"); // turn servo to the left
      break;

    } else if (inChar == 'R') { // turn servo to the right

      sparki.servo(SERVO_RIGHT);
      Serial.println("SERVO_RIGHT");
      break;

    } else if (inChar == 'C') { // turn servo to the middle

      sparki.servo(SERVO_CENTER);
      Serial.println("SERVO_CENTER");
      break;
    } else if (inChar == 'W') { //move  forward

      sparki.moveForward(5);
      Serial.println("MOVE_FORWARD");

    } else if (inChar == 'S') { //move back

      sparki.moveBackward(5);
      Serial.println("MOVE_BACKWARD");


    } else if (inChar == 'I') { //toggle for Inverse Kinematics
      Serial.println("INVERSE_KINEMATICS");
      inverseKinematics(0.1, 0.1);

    } else if (inChar == 'O'){  //toggle for Odometry
      Serial.println("ODOMETRY");
      odometry();

    } else if (inChar == 'B'){  //toggle for reactiveBehaviors
      Serial.println("REACTIVE_BEHAIVIORS");
      reactiveBehaviors();

    } else if (inChar == 'A'){ // toggle for line follow **optional**

          sparki.moveLeft(5);
          Serial.println("MOVE_LEFT");
      
     } else if (inChar == 'D'){ // toggle for line follow **optional**

          sparki.moveRight(5);
          Serial.println("MOVE_RIGHT");

    } else {
      return;
    }
  }
}

int inverseKinematics(float xG, float yG) {
  float xI                = 0.0;
  float yI                = 0.0;
  float thetaR            = 0.0;

  // float xG                = 0.15; // 15cm
  // float yG                = 0.15;
  float thetaG            = 0.0;

  float n                 = 0.0;
  float rho               = 0.0;
  float alpha             = 0.0;

  float xR                = 0.0;
  float thetaRP           = 0.0;

  float rVel;
  float lVel;
  float avgVel            = VEL;

  unsigned long int startTime;
 // sparki.clearLCD();

  startTime = millis();

  // Translate into real-world coordinate frame
  int xPoint = (xI / 10) + 50;
  int yPoint = (yI / 10) + 50;

  // Map out the path of Sparki
  sparki.drawPixel(xPoint, yPoint);
  //   sparki.updateLCD();

  // *******************************************************
  // * Feedback Controller

  rho = sqrt(pow((xI - xG), 2) + pow((yI - yG), 2));
  alpha = thetaR - atan2((yI - yG), (xI - xG)) - PI/2.0;
  n = thetaG - thetaR;

  Serial.print("rho: "); Serial.print(rho);
  Serial.print(" alpha: "); Serial.print(alpha / PI * 180.0);
  Serial.print(" n: "); Serial.println(n); Serial.println("- - - -");

  // Forward speed
  xR = 0.1 * rho;

  // Rotational speed
  thetaRP = 0.1 * (alpha); // + 0.01 * n;

  // *******************************************************
  // * Inverse Kinematics [Formula 3.64]

  lVel = (2.0 * xR / RAD - thetaRP * AXLE_DIST / RAD) / 2.0;
  rVel = (2.0 * xR / RAD + thetaRP * AXLE_DIST / RAD) / 2.0;

  // Serial.print("L: " + lVel + " "); Serial.print("R: " + rVel);

  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, lVel / (VEL / RAD) * 100.0);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, rVel / (VEL / RAD) * 100.0);

  // *******************************************************
  // * Odometry [Formula 3.40]

  // Average rotation speeds of the L & R wheels (phi * radius)
  avgVel = (rVel * RAD + lVel * RAD) * 0.5;

  xI += cos(thetaR) * avgVel * LOOP_TIME / 1000.0;
  yI += sin(thetaR) * avgVel * LOOP_TIME / 1000.0;
  thetaR += (rVel * RAD - lVel * RAD) / AXLE_DIST * LOOP_TIME / 1000.0;

  Serial.print("xI: "); Serial.print(xI);
  Serial.print(" yI: "); Serial.print(yI);
  Serial.print(" thetaR: "); Serial.println(thetaR/PI * 180.0);

  // Ensure every loop is exactly 100 ms
  while (millis() < startTime + LOOP_TIME) {}

  return 1;
}

float* odometry() {
  float x                 = 0.0;
  float y                 = 0.0;
  float theta             = 0.0;

  float rVel              = 0.0;
  float lVel              = 0.0;
  float avgVel            = 0.0;
  float coords[2];

  int startTime, endTime;

  startTime = millis();
  // Average rotation speeds of the L & R wheels
  avgVel = (rVel + lVel) * 0.5;

  // Formula 3.40
  x += cos(theta) * avgVel * LOOP_TIME_O;
  y += sin(theta) * avgVel * LOOP_TIME_O;
  theta += (rVel - lVel) / AXLE_DIST * LOOP_TIME_O;

  // Translate into real-world coordinate frame
  int xPoint = (x / 10) + 50;
  int yPoint = (y / 10) + 50;

  // Map out the path of Sparki
  // sparki.drawPixel(xPoint, yPoint);
  // sparki.updateLCD();

  endTime = millis();

  Serial.print("X: ");
  Serial.println(x / 10);
  Serial.print("Y: ");
  Serial.println(y / 10);

  Serial.print("Theta: ");
  Serial.println(theta);

  Serial.print("Time: ");
  Serial.println(LOOP_TIME - (endTime - startTime));
  Serial.println("- - - - - - - - - - - - - - -");

  // Ensure every loop is 100ms  --see if we need to delete this
  if (LOOP_TIME - (endTime - startTime) > 0) {
    delay(LOOP_TIME - (endTime - startTime));
  }

  coords[0] = x;
  coords[1] = y;
  Serial.print(x, y);
  return coords;
}


int reactiveBehaviors(){
//   Move forward
  int cm = sparki.ping();
  sparki.moveForward();
  sparki.print(cm);
  if(cm == 0.3){
//     Turn around
    delay(1000);
    sparki.moveRight(180);
    sparki.moveForward();
  }
  
}
