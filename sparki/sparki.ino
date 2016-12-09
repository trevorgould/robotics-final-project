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
<<<<<<< HEAD
//       Pass in coordinates
      inverseKinematics(1.0, 2.0);

=======
      inverseKinematics(0.15, 0.35);
>>>>>>> 66a4639ae7cf799788bf28f55064ec47c998e049

    } else if (inChar == 'O'){  //toggle for Odometry
      Serial.println("ODOMETRY");
      odometry();
      
    } else if (inChar == 'B'){  //toggle for reactiveBehaviors
      Serial.println("REACTIVE_BEHAIVIORS");
     // reactiveBehaviors();
      
    } else if (inChar == 'T'){ // toggle for line follow **optional**

      // lineFollow();

    } else {
      return;
    }
    
//      if (LOOP_TIME - (endTime - startTime) > 0) {
//       delay(LOOP_TIME - (endTime - startTime));
//     }
  }
}

int inverseKinematics(float x_goal, float y_goal) {
  float xI                = 0.0;
  float yI                = 0.0;
  float thetaR            = 0.0;

<<<<<<< HEAD
  float xG                = x_goal; // Passed values
  float yG                = y_goal;
=======
  // float xG                = 0.15; // 15cm
  // float yG                = 0.15;
>>>>>>> 66a4639ae7cf799788bf28f55064ec47c998e049
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

  startTime = millis();

  // Translate into real-world coordinate frame
  int xPoint = (xI / 10) + 50;
  int yPoint = (yI / 10) + 50;


  // *******************************************************
  // * Feedback Controller

  rho = sqrt(pow((xI - xG), 2) + pow((yI - yG), 2));
  alpha = thetaR - atan2((yI - yG), (xI - xG)) - PI/2.0;
  n = thetaG - thetaR;

  sparki.print("rho: "); sparki.print(rho);
  sparki.print(" alpha: "); sparki.print(alpha / PI * 180.0);
  sparki.print(" n: "); sparki.print(n); sparki.println("- - - -");
  


  // Forward speed
  xR = 0.1 * rho;

  // Rotational speed
  thetaRP = 0.1 * (alpha); // + 0.01 * n;

  // *******************************************************
  // * Inverse Kinematics [Formula 3.64]

  lVel = (2.0 * xR / RAD - thetaRP * AXLE_DIST / RAD) / 2.0;
  rVel = (2.0 * xR / RAD + thetaRP * AXLE_DIST / RAD) / 2.0;

  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, lVel / (VEL / RAD) * 100.0);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, rVel / (VEL / RAD) * 100.0);

  // *******************************************************
  // * Odometry [Formula 3.40]

  // Average rotation speeds of the L & R wheels (phi * radius)
  avgVel = (rVel * RAD + lVel * RAD) * 0.5;

  xI += cos(thetaR) * avgVel * LOOP_TIME / 1000.0;
  yI += sin(thetaR) * avgVel * LOOP_TIME / 1000.0;
  thetaR += (rVel * RAD - lVel * RAD) / AXLE_DIST * LOOP_TIME / 1000.0;

  sparki.print("xI: "); sparki.print(xI);
  sparki.print(" yI: "); sparki.print(yI);
  sparki.print(" thetaR: "); sparki.println(thetaR/PI * 180.0);

  // Ensure every loop is exactly 100 ms
  //while (millis() < startTime + LOOP_TIME) {}

  return 1;
}

//server will receive from odometry
int odometry() {
  float x                 = 0.0;
  float y                 = 0.0;
  float theta             = 0.0;

  float rVel              = 0.0;
  float lVel              = 0.0;
  float avgVel            = 0.0;

  int startTime, endTime;
//  int lineLeft, lineCenter, lineRight;

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
//    
  
  return 1;
}
/*
int reactiveBehaviors() {
  int state = DRIVE;
  bool gotObj = false;

  int cm = sparki.ping();

  Serial.println(cm);

  switch (state) {
  
//     case DRIVE:
//       sparki.moveForward();
//       if (!gotObj) sparki.gripperOpen();
//       if (cm == 3) state = GRAB;
//       break;

    //case GRAB:
    //  sparki.RGB(RGB_RED);
    //  sparki.gripperStop();
    //  sparki.moveStop();
    //  delay(1000);
    //  sparki.moveForward(1);
    //  delay(1000);
    //  sparki.moveStop();
    //  sparki.gripperClose();
    //  delay(3000);
    //  sparki.gripperStop();
    //  gotObj = true;
    //  state = ROTATE;
    //  break;

   // case ROTATE:
    //  delay(1000);
     // sparki.moveRight(180);
      //state = FOLLOW;
      //break;
// Line Following
//     case FOLLOW: {
//       sparki.moveForward();
//       int threshold = 500;
//       int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
//       int lineCenter = sparki.lineCenter(); // measure the center IR sensor
//       int lineRight  = sparki.lineRight();  // measure the right IR sensor

//       if ( lineLeft < threshold ) sparki.moveLeft();
//       if ( lineRight < threshold ) sparki.moveRight();
//       if ( ((lineLeft + lineRight + lineCenter) / 3) < 195 ) state = DROP;

//       // if the center line sensor is the only one reading a line
//       if ( (lineCenter < threshold) && (lineLeft > threshold) && (lineRight > threshold) ) {
//         sparki.moveForward();
//       }

    //  sparki.clearLCD();
      
     // sparki.println(lineLeft);
     // sparki.println(lineCenter);
     // sparki.println(lineRight);
     // sparki.println((lineLeft + lineRight + lineCenter) / 3);
     // sparki.updateLCD(); // display all of the information written to the screen
    //  delay(100); // wait 0.1 seconds
     // break;
   // }

  //  case DROP:
  //   sparki.moveStop();
  //   sparki.gripperOpen();
  //   break;
  }

  return 1;
}
*/
