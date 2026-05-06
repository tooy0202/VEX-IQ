#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain Brain;

// define your global instances of motors and other devices here
inertial BrainInertial = inertial();
controller Controller = controller();
motor MotorPinMotorA = motor(PORT3, false);
motor MotorPinMotorB = motor(PORT9, true);
motor_group MotorPin = motor_group(MotorPinMotorA, MotorPinMotorB);

motor MotorBeamMotorA = motor(PORT5, false);
motor MotorBeamMotorB = motor(PORT11, true);
motor_group MotorBeam = motor_group(MotorBeamMotorA, MotorBeamMotorB);

touchled TouchLED = touchled(PORT6);
pneumatic Pneumatic_Pin_Beam = pneumatic(PORT4);
motor MotorLeft = motor(PORT12, false);
motor MotorRight = motor(PORT10, true);
pneumatic guide = pneumatic(PORT8);
distance D1 = distance(PORT1);
distance D2 = distance(PORT2);

// AI Classification Competition Element IDs - Mix & Match
enum gameElementsMixAndMatch
{
  beam,
  bluePin,
  redPin,
  orangePin,
};

// AI Vision Code Descriptions
vex::aivision AIVision7(PORT7, aivision::ALL_AIOBJS);

// generating and setting random seed
void initializeRandomSeed()
{
  wait(100, msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  int seed = int(xAxis + yAxis + zAxis);
  srand(seed);
}

void vexcodeInit()
{
  initializeRandomSeed();
}

bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

// function Beam
void R3F();
void Grab_Beam_up();
void Place_beam();
void Drop_down_beam();
void Place_Standoff(int B);
// function pin
void Drop_down();
void Drop_down_Grab_Up();
void Drop_down_Grab_Up_mod();
void Grab_then_up();
// function Move and turn
void Movemen_v_fb_d(double velocity, int FB, double distance);
double OldComputeTurnSpeed(double absErr, double maxSpeed);
void StopMove_v_fb(int velocity, int FB);
double hypotenuse(double a, double b);
void TurnLeft_h_v_m_v1(double heading, double velocity, double momentum);
void TurnRight_h_v_m_v1(double heading, double velocity, double momentum);
void dis_heading_Align(double velocity);
void Go_To_Pin(int Gopin);
// thread
int PrintCon();
void controArm();
int take_off();

int Screen_precision = 0, Console_precision = 0, AIVision7_objectIndex = 0, LiftBeam = 0;
bool startgame, beamon, touched = true, armfont = false, armback = false, Arm_Grab_pin = false, armfontF = false;
double d_now_go;

// ========== FUNTION BEAM ==========

void Drop_down_beam()
{
  guide.extend(cylinder1);
  wait(100, msec);
  MotorBeam.setStopping(hold);
  beamon ? MotorBeam.setVelocity(100, percent) : MotorBeam.setVelocity(45, percent);
  MotorBeam.setMaxTorque(100, percent);
  MotorBeam.spin(forward);
  wait(0.5, seconds);
  while (MotorBeam.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorBeam.spin(forward);
  }
  MotorBeam.setStopping(brake);
  MotorBeam.setMaxTorque(100, percent);
  MotorBeam.stop();
  MotorBeam.stop(coast);
}

void Place_beam()
{
  MotorLeft.stop();
  MotorRight.stop();
  MotorBeam.setVelocity(45.0, percent);
  guide.extend(cylinder1);
  wait(100, msec);
  MotorBeam.spinFor(forward, 160.0, degrees, true);
  Pneumatic_Pin_Beam.retract(cylinder2);
  MotorBeam.spinFor(reverse, 100.0, degrees, false);
  MotorLeft.setVelocity(85.0, percent);
  MotorRight.setVelocity(85.0, percent);
  MotorLeft.spinFor(forward, 130.0, degrees, false);
  MotorRight.spinFor(forward, 130.0, degrees, false);
}

void Grab_Beam_up()
{
  MotorBeam.setStopping(hold);
  MotorBeam.spinFor(reverse, 50.0, degrees, false);
  wait(0.8, seconds);
  guide.retract(cylinder1);
  wait(0.7, seconds);
  MotorBeam.stop();
}

void Place_Standoff(int B)
{
  MotorBeam.setMaxTorque(100, percent);
  if (B == 0)
  {
    MotorBeam.setVelocity(100, percent);
    MotorBeam.spinFor(forward, 90, degrees, true);
    Pneumatic_Pin_Beam.retract(cylinder2);
    wait(0.3, seconds);
    MotorBeam.spinFor(reverse, 60, degrees, false);
  }
  else
  {
    MotorBeam.setVelocity(45, percent);
    MotorBeam.spinFor(forward, 85, degrees, true);
    Pneumatic_Pin_Beam.retract(cylinder2);
    wait(0.3, seconds);
    MotorBeam.spinFor(reverse, 100, degrees, false);
  }
}

// ========== FUNTION MOVE AND TURN ==========

void StopMove_v_fb(int velocity, int FB)
{
  MotorLeft.setVelocity(velocity, percent);
  MotorRight.setVelocity(velocity, percent);
  if (FB == 0)
  {
    MotorRight.spin(forward);
    MotorLeft.spin(forward);
    wait(0.4, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      MotorRight.spin(forward);
      MotorLeft.spin(forward);
    }
    MotorRight.stop();
    MotorLeft.stop();
    return;
  }

  if (FB == 1)
  {
    MotorRight.spin(reverse);
    MotorLeft.spin(reverse);
    wait(0.4, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      MotorRight.spin(reverse);
      MotorLeft.spin(reverse);
    }
    MotorRight.stop();
    MotorLeft.stop();
    return;
  }
}

void Movemen_v_fb_d(double velocity, int FB, double distance)
{
  double K = 1.0;
  MotorLeft.setVelocity((velocity * K), percent);
  MotorRight.setVelocity((velocity * K), percent);
  MotorLeft.setStopping(coast);
  MotorRight.setStopping(coast);
  if (FB == 0)
  {
    MotorLeft.spinFor(forward, (distance / 0.833), degrees, false);
    MotorRight.spinFor(forward, (distance / 0.833), degrees, true);
    while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
    {
      wait(10, msec);
    }
  }
  if (FB == 1)
  {
    MotorLeft.spinFor(reverse, (distance / 0.833), degrees, false);
    MotorRight.spinFor(reverse, (distance / 0.833), degrees, true);
    while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
    {
      wait(10, msec);
    }
  }
}

double hypotenuse(double a, double b)
{
  return sqrt(a * a + b * b);
}

void dis_heading_Align(double velocity)
{

  double current = BrainInertial.heading();
  double d = 615.95;
  double distance = 0;
  double Hypotenuse = 0;

  MotorLeft.stop(brake);
  MotorRight.stop(brake);

  if (current <= 45 || current >= 315)
  {
    double theta;

    if (current <= 45)
      theta = current;
    else
      theta = 360 - current; // 315–360 → 0–45

    double rad = theta * M_PI / 180.0;
    d = 1253 / cos(rad);
  }
  d_now_go = d - 1253;
  if (current > 3 && current <= 45)
  {
    Hypotenuse = hypotenuse(d_now_go, 370);
    if (Hypotenuse < 5)
    {
      Hypotenuse = 370;
    }
    Movemen_v_fb_d(100, 0, Hypotenuse);
    TurnRight_h_v_m_v1(90, 80, 4);
  }
  else if (current < 357 && current >= 315)
  {
    Hypotenuse = hypotenuse(d_now_go, 370);
    if (Hypotenuse < 5)
    {
      Hypotenuse = 370;
    }
    Movemen_v_fb_d(100, 0, Hypotenuse);
    TurnRight_h_v_m_v1(90, 80, 4);
  }
  else
  {
    Movemen_v_fb_d(velocity, 0, 370);
    TurnRight_h_v_m_v1(90, 80, 4);
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    distance += D2.objectDistance(mm);
  }
  distance = distance / 100;
  double dis_Move = fabs(1153 - distance);
  if (current < 357 && current >= 315)
  {
    Movemen_v_fb_d(100, 0, dis_Move);
  }
  if (current > 3 && current <= 45)
  {
    Movemen_v_fb_d(100, 1, dis_Move);
  }
}

double OldComputeTurnSpeed(double absErr, double maxSpeed)
{
  double minSpeed = maxSpeed * 0.2;
  double maxErrForScale = 90.0;

  double t = absErr / maxErrForScale;
  if (t > 1.0)
    t = 1.0;

  // curve: make small errors give much smaller speeds
  t = t * t; // square makes the curve “steeper” near 0

  double speed = minSpeed + (maxSpeed - minSpeed) * t;

  // Cap speed
  if (speed > 90)
    speed = 90;

  return speed;
}

void TurnLeft_h_v_m_v1(double heading, double velocity, double momentum)
{
  while (true)
  {
    double current = BrainInertial.heading();
    double error = heading - current; // positive → need left turn
    double absErr = fabs(error);

    if (absErr <= momentum)
    {
      MotorLeft.setStopping(brake);
      MotorRight.setStopping(brake);
      MotorLeft.stop();
      MotorRight.stop();

      while (fabs(MotorLeft.velocity(vex::velocityUnits::pct)) > 1 ||
             fabs(MotorRight.velocity(vex::velocityUnits::pct)) > 1)
        wait(10, msec);

      MotorLeft.setStopping(coast);
      MotorRight.setStopping(coast);
      printf("Final heading: %.2f\n", BrainInertial.heading());
      break;
    }

    double turnSpeed = OldComputeTurnSpeed(absErr, velocity);
    MotorLeft.setVelocity(turnSpeed, percent);
    MotorRight.setVelocity(turnSpeed, percent);

    MotorLeft.spin(reverse);
    MotorRight.spin(forward);

    wait(20, msec);
  }
  wait(0.1, seconds);
}

void TurnRight_h_v_m_v1(double heading, double velocity, double momentum)
{
  while (true)
  {
    double current = BrainInertial.heading();
    double error = heading - current; // positive → need left turn
    double absErr = fabs(error);

    if (absErr <= momentum)
    {
      MotorLeft.setStopping(brake);
      MotorRight.setStopping(brake);
      MotorLeft.stop();
      MotorRight.stop();

      while (fabs(MotorLeft.velocity(vex::velocityUnits::pct)) > 1 ||
             fabs(MotorRight.velocity(vex::velocityUnits::pct)) > 1)
        wait(10, msec);

      MotorLeft.setStopping(coast);
      MotorRight.setStopping(coast);
      printf("Final heading: %.2f\n", BrainInertial.heading());
      break;
    }

    double turnSpeed = OldComputeTurnSpeed(absErr, velocity);
    MotorLeft.setVelocity(turnSpeed, percent);
    MotorRight.setVelocity(turnSpeed, percent);

    MotorLeft.spin(forward);
    MotorRight.spin(reverse);

    wait(20, msec);
  }
  wait(0.1, seconds);
}

// ========== FUNTION PIN ==========

void Grab_then_up()
{
  MotorPin.resetPosition();
  MotorPin.setPosition(0.0, degrees);
  MotorPin.setStopping(hold);
  Pneumatic_Pin_Beam.extend(cylinder1);
  wait(0.3, seconds);
  MotorPin.spinToPosition(180.0, degrees, false);
  wait(0.2, seconds);
  guide.extend(cylinder2);
}

void Drop_down()
{
  MotorPin.resetPosition();
  MotorLeft.setStopping(hold);
  MotorRight.setStopping(hold);
  wait(0.35, seconds);
  MotorPin.setVelocity(100, percent);
  MotorPin.spinFor(reverse, 50.0, degrees, false);
  wait(0.1, seconds);
  guide.retract(cylinder2);
  MotorPin.spinFor(reverse, 170.0, degrees, false);
  wait(250, msec);
  MotorPin.setVelocity(100, percent);
  MotorPin.spin(reverse);
  while (MotorPin.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorPin.spin(reverse);
    wait(100, msec);
  }
  Pneumatic_Pin_Beam.retract(cylinder1);
  MotorPin.setStopping(coast);
  MotorPin.stop(coast);
  MotorLeft.setStopping(coast);
  MotorRight.setStopping(coast);
  MotorPin.setPosition(0.0, degrees);
}

void Drop_down_Grab_Up_mod()
{
  MotorPin.resetPosition();
  MotorPin.setPosition(0.0, degrees);
  MotorPin.setStopping(hold);
  MotorPin.spinToPosition(-50.0, degrees, false);
  wait(0.1, seconds);
  guide.retract(cylinder2);
  MotorPin.spinToPosition(-170.0, degrees, false);
  wait(0.25, seconds);
  Pneumatic_Pin_Beam.retract(cylinder1);
  while (MotorPin.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorPin.spin(reverse);
  }
  wait(0.2, seconds);
  MotorLeft.setVelocity(50.0, percent);
  MotorRight.setVelocity(50.0, percent);
  MotorLeft.spinFor(forward, 120.0, degrees, false);
  MotorRight.spinFor(forward, 120.0, degrees, false);
  wait(0.3, seconds);
  Pneumatic_Pin_Beam.extend(cylinder1);
  wait(0.3, seconds);
  MotorPin.resetPosition();
  MotorPin.spinToPosition(215.0, degrees, false);
  wait(0.2, seconds);
  guide.extend(cylinder2);
}

void Drop_down_Grab_Up()
{
  wait(0.1, seconds);
  MotorPin.resetPosition();
  MotorPin.setPosition(0.0, degrees);
  MotorPin.setStopping(hold);
  MotorPin.spinToPosition(-50.0, degrees, false);
  wait(0.1, seconds);
  guide.retract(cylinder2);
  MotorPin.spinToPosition(-170.0, degrees, false);
  wait(0.25, seconds);
  Pneumatic_Pin_Beam.retract(cylinder1);
  while (MotorPin.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorPin.spin(reverse);
    wait(1, msec);
  }
  MotorLeft.setVelocity(50.0, percent);
  MotorRight.setVelocity(50.0, percent);
  MotorLeft.spinFor(forward, 500.0, degrees, false);
  MotorRight.spinFor(forward, 500.0, degrees, false);
  wait(0.2, seconds);
  Pneumatic_Pin_Beam.extend(cylinder1);
  wait(0.3, seconds);
  MotorLeft.stop(brake);
  MotorRight.stop(brake);
  MotorPin.resetPosition();
  MotorPin.spinToPosition(215.0, degrees, false);
  wait(0.2, seconds);
  guide.extend(cylinder2);
}

void R3F()
{
  guide.retract(cylinder2);
  Pneumatic_Pin_Beam.extend(cylinder2);
  MotorPin.spinFor(forward, 800.0, degrees, false);
  wait(1500, msec);
  Pneumatic_Pin_Beam.retract(cylinder1);
  wait(0.1, seconds);
  armfont = true;
}

// ========== FUNTION AI CAM ==========

// double ComputeApproachSpeed(double width, double maxSpeed)
// {
//   // Tune these three numbers for your robot & camera
//   const double targetWidth =
//       85.0; // how big the pin should look when you want to stop
//   const double minDetectWidth =
//       20.0; // typical width when it's "far but visible"

//   double minSpeed = maxSpeed * 0.2; // don't go slower than 20% of max
//   double maxErr = targetWidth - minDetectWidth;

//   // Error = how much smaller than target the current width is
//   double err = targetWidth - width;

//   // Clamp error between 0 and maxErr
//   if (err < 0)
//     err = 0;
//   if (err > maxErr)
//     err = maxErr;

//   // Normalize → 0..1
//   double t = err / maxErr;

//   // Curve: square to make it gentler near target
//   t = t * t;

//   // When far (err large) → t ~1 → speed ~maxSpeed
//   // When close (err small) → t ~0 → speed ~minSpeed
//   double speed = minSpeed + (maxSpeed - minSpeed) * t;
//   return speed;
// }

void Go_To_Pin(int Gopin)
{
  const double targetCenY = 120.0; // when object is this big, we stop
  const double maxSpeed = 100.0;   // max forward speed (tune if needed)

  while (true)
  {
    AIVision7.takeSnapshot(aivision::ALL_AIOBJS);

    int bestIndex = -1;
    int maxWidth = 0;

    // 1) Find the closest (widest) object with the requested ID
    for (int i = 0; i < AIVision7.objectCount; i++)
    {
      if (AIVision7.objects[i].id == Gopin &&
          AIVision7.objects[i].width > maxWidth)
      {
        maxWidth = AIVision7.objects[i].width;
        bestIndex = i;
      }
    }

    if (bestIndex != -1)
    {
      double cenY = AIVision7.objects[bestIndex].centerY;

      // 2) Compute speed from width, like ComputeTurnSpeed
      // double speed = ComputeApproachSpeed(width, maxSpeed);

      MotorLeft.setVelocity(maxSpeed, percent);
      MotorRight.setVelocity(maxSpeed, percent);
      MotorLeft.spin(forward);
      MotorRight.spin(forward);

      // 3) Close enough → brake and exit
      if (cenY >= targetCenY)
      {
        break;
      }
    }
    else
    {
      // No object found → stop and keep searching
      MotorLeft.stop();
      MotorRight.stop();
    }

    wait(10, msec); // small delay to avoid hammering the CPU
  }
}

//========================= START ROBOT ===========================
int take_off()
{
  startgame = true;
  beamon = true;
  MotorPin.setPosition(0.0, degrees);
  MotorBeam.setPosition(0.0, degrees);
  MotorPin.setVelocity(80.0, percent);
  MotorPin.setMaxTorque(100.0, percent);
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
  MotorLeft.spin(reverse, 2, pct);
  MotorRight.spin(reverse, 2, pct);
  MotorPin.setStopping(hold);
  MotorBeam.setStopping(coast);
  Pneumatic_Pin_Beam.retract(cylinder1);
  Pneumatic_Pin_Beam.retract(cylinder2);
  guide.extend(cylinder1);
  Pneumatic_Pin_Beam.pumpOn();
  TouchLED.setColor(red);
  Brain.Screen.setCursor(1, 1);
  guide.retract(cylinder2);
  Drop_down();
  Drop_down_beam();
  MotorBeam.spinFor(reverse, 520.0, degrees);
  Pneumatic_Pin_Beam.extend(cylinder2);
  beamon = false;
  return 0;
}

//===================== Dont wait FUNTION ======================
void controArm()
{
  while (true)
  {
    if (armfontF)
    {
      guide.retract(cylinder2);
      Pneumatic_Pin_Beam.extend(cylinder2);
      MotorPin.spinFor(forward, 800.0, degrees, false);
      wait(1500, msec);
      Pneumatic_Pin_Beam.retract(cylinder1);
      MotorPin.setStopping(coast);
      MotorPin.spinFor(reverse, 700.0, degrees, false);
      armfontF = false;
    }
    if (armfont)
    {
      guide.retract(cylinder2);
      R3F();
      MotorPin.setStopping(coast);
      MotorPin.spinFor(reverse, 700.0, degrees, false);
      wait(1000, msec);
      MotorBeam.spinFor(reverse, 200, degrees, false);
      MotorPin.stop(coast);
      armfont = false;
    }
    if (armback)
    {
      Drop_down_beam();
      armback = false;
    }
    if (Arm_Grab_pin)
    {
      MotorPin.setStopping(hold);
      Pneumatic_Pin_Beam.extend(cylinder1);
      MotorPin.spinFor(forward, 180.0, degrees, true);
      guide.extend(cylinder2);
      MotorPin.stop();
      Arm_Grab_pin = false;
    }

    wait(10, msec);
  }
}

//================================ Print ===============================
int PrintCon()
{
  while (true)
  {
    Brain.Screen.clearScreen();
    if (startgame)
    {
      if (Brain.timer(seconds) >= 59)
      {
        Pneumatic_Pin_Beam.retract(cylinder1);
        MotorLeft.stop(hold);
        MotorRight.stop(hold);
        Brain.programStop();
      }
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("start");
    }
    else
    {
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("D2: %.2f", D2.objectDistance(mm));
    }
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("%.2f", BrainInertial.heading(degrees));

    int h = BrainInertial.heading();

    printf("Heading: %.d deg\n", h);
    wait(50, msec);
  }
  return 0;
}

// =================== TOUCH LED EVENT ===================
void Autonomous()
{
  MotorLeft.stop(coast);
  MotorRight.stop(coast);
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
  MotorLeft.setMaxTorque(100.0, percent);
  MotorRight.setMaxTorque(100.0, percent);
  MotorLeft.setVelocity(100.0, percent);
  MotorRight.setVelocity(100.0, percent);
  startgame = false;
  Brain.Timer.reset();

  TouchLED.setColor(purple);

  if (touched)
  {
    BrainInertial.setHeading(0.0, degrees);
    BrainInertial.setRotation(0.0, degrees);
    wait(1, seconds);
    touched = false;
  }
  else
  {
    // ========================= Initial ===============================
    TouchLED.setColor(purple);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    startgame = true;
    Brain.Timer.reset();
    BrainInertial.setRotation(0.0, degrees);
    // ====================== Fist 2 Pins ==============================
    Movemen_v_fb_d(100, 0, 900.0);
    Movemen_v_fb_d(20, 0, 100.0);
    Pneumatic_Pin_Beam.extend(cylinder1);
    wait(0.3, seconds);

    //====================== Init Second 2 Pins ========================
    Movemen_v_fb_d(100, 1, 410);

    Arm_Grab_pin = true;
    TurnRight_h_v_m_v1(59, 70, 3);
    Movemen_v_fb_d(100, 0, 765);
    MotorLeft.spin(forward, 40, pct);
    MotorRight.spin(forward, 30, pct);
    wait(500, msec);
    while (MotorLeft.velocity(percent) > 0)
    {
      double speedf = MotorLeft.velocity(percent);
      MotorLeft.setVelocity(speedf - 10, percent);
      double speedr = MotorRight.velocity(percent);
      MotorRight.setVelocity(speedr - 10, percent);
      MotorLeft.spin(forward);
      MotorRight.spin(forward);
      wait(50, msec);
    }
    MotorLeft.stop(coast);
    MotorRight.stop(coast);
    Drop_down_Grab_Up();
    Movemen_v_fb_d(100, 1, 249);
    TurnLeft_h_v_m_v1(0, 80.0, 4);
    // =============== Place Pin on Standoff =======================
    MotorRight.spin(reverse, 100, pct);
    MotorLeft.spin(reverse, 100, pct);
    wait(0.4, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      if (D2.objectDistance(mm) > 100)
      {
        MotorLeft.setVelocity(100, pct);
        MotorRight.setVelocity(100, pct);
      }
      else
      {
        MotorLeft.setVelocity(30, pct);
        MotorRight.setVelocity(30, pct);
      }
      MotorRight.spin(reverse);
      MotorLeft.spin(reverse);
    }
    MotorLeft.stop(coast);
    MotorRight.stop(coast);
    Place_Standoff(0);
    // // // ========================= U Beam =====================================
    //--------- New logic
    double Next_turn = BrainInertial.heading();
    Movemen_v_fb_d(100, 0, 100);
    if (Next_turn > 180 && Next_turn < 357)
    {
      Next_turn = (360 - Next_turn) + 180;
    }
    else if (Next_turn > 3 && Next_turn < 180)
    {
      Next_turn = fabs((360 - Next_turn) - 180);
    }
    else
    {
      Next_turn = 180;
    }
    // // ========================= U Beam =====================================
    armback = true;
    TurnRight_h_v_m_v1(Next_turn, 80, 3);
    MotorRight.setVelocity(100, percent);
    MotorLeft.setVelocity(100, percent);
    MotorRight.spin(reverse);
    MotorLeft.spin(reverse);
    wait(0.4, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      MotorRight.spin(reverse);
      MotorLeft.spin(reverse);
    }
    wait(0.2, seconds);
    Pneumatic_Pin_Beam.extend(cylinder1);
    wait(0.15, seconds);
    MotorLeft.stop();
    MotorRight.stop();
    armfont = true;
    wait(200, msec);
    //===================== Y stack On Standoff ===============
    MotorLeft.setVelocity(70, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(800, msec);
    MotorLeft.stop(brake);
    MotorRight.stop(brake);
    TurnLeft_h_v_m_v1(3, 70, 3);
    //-------End New logic
    MotorBeam.spinFor(reverse, 260, degrees, false);
    wait(200, msec);
    MotorRight.spin(reverse, 100, pct);
    MotorLeft.spin(reverse, 100, pct);
    wait(0.2, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      MotorRight.spin(reverse, 100, pct);
      MotorLeft.spin(reverse, 100, pct);
      wait(10, msec);
    }
    wait(300, msec);
    Place_Standoff(1);
    MotorLeft.stop(coast);
    MotorRight.stop(coast);
    // ====================== PART TWO ==================================
    // ====================== First 2 Pins ==============================
    dis_heading_Align(100);
    wait(0.1, seconds);
    armback = true;
    MotorBeam.setStopping(coast);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    Pneumatic_Pin_Beam.extend(cylinder1);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(0.5, seconds);
    Pneumatic_Pin_Beam.retract(cylinder1);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(0.6, seconds);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(54, percent);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(1.1, seconds);
    MotorLeft.setVelocity(9, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(0.3, seconds);
    MotorLeft.setStopping(hold);
    MotorRight.setStopping(hold);
    MotorLeft.setVelocity(80, percent);
    MotorRight.setVelocity(80, percent);
    MotorRight.spin(forward);
    MotorLeft.spin(forward);
    wait(0.5, seconds);
    while (MotorLeft.velocity(vex::velocityUnits::pct) != 0)
    {
      MotorRight.spin(forward);
      MotorLeft.spin(forward);
    }
    wait(0.3, seconds);
    MotorRight.stop();
    MotorLeft.stop();
    Pneumatic_Pin_Beam.extend(cylinder1);
    Pneumatic_Pin_Beam.retract(cylinder2);
    wait(0.2, seconds);
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    // // // ====================== Second 2 Pins ==============================
    MotorPin.spinFor(forward, 180.0, degrees, false);
    Movemen_v_fb_d(100, 1, 100);
    guide.extend(cylinder2);
    TurnRight_h_v_m_v1(205, 80, 4);
    Movemen_v_fb_d(100, 0, 330);
    Movemen_v_fb_d(35, 0, 200);
    Drop_down_Grab_Up_mod();
    wait(0.2, seconds);
    // // ====================== U Beam ==============================
    Movemen_v_fb_d(100, 1, 230);
    TurnRight_h_v_m_v1(285, 80, 4);
    StopMove_v_fb(100, 1);
    Pneumatic_Pin_Beam.extend(cylinder2);
    armfontF = true;
    Movemen_v_fb_d(100, 0, 100);
    Movemen_v_fb_d(100, 0, 85);
    // // // ====================== Y Beam ==============================
    TurnLeft_h_v_m_v1(183, 80, 4);
    Drop_down();
    Movemen_v_fb_d(40, 0, 150);
    Movemen_v_fb_d(20, 0, 100);
    MotorBeam.spinFor(reverse, 300, degrees, false);
    Grab_then_up();
    wait(0.1, seconds);
    StopMove_v_fb(60, 0);
    Movemen_v_fb_d(100, 1, 50);
    Drop_down();
    wait(100, msec);
    MotorLeft.spin(reverse, 40, pct);
    MotorRight.spin(reverse, 100, pct);
    wait(0.5, seconds);
    MotorLeft.spin(reverse, 100, pct);
    MotorRight.spin(reverse, 100, pct);
    wait(0.3, sec);
    TurnLeft_h_v_m_v1(15, 80, 4);
    StopMove_v_fb(70, 1);
    MotorRight.spin(reverse, 100, pct);
    wait(0.3, seconds);
    MotorLeft.spin(reverse, 100, pct);
    wait(0.1, seconds);
    Movemen_v_fb_d(50, 0, 90);
    MotorLeft.stop(coast);
    MotorRight.stop(coast);
    Place_beam();
    // part-3------------------------------------------------------------
    Movemen_v_fb_d(100, 0, 350);
    TurnLeft_h_v_m_v1(270, 80, 4);
    MotorLeft.stop(brake);
    MotorRight.stop(brake);
    MotorLeft.spin(forward, 100, pct);
    MotorRight.spin(forward, 100, pct);
    wait(800, msec);
    Pneumatic_Pin_Beam.extend(cylinder1);
    Arm_Grab_pin = true;
    wait(1000, msec);
    Pneumatic_Pin_Beam.retract(cylinder1);
    wait(1, sec);
    MotorLeft.stop(hold);
    MotorRight.stop(hold);
  }
}

int main()
{
  vexcodeInit();
  thread printco(PrintCon);
  thread controarm(controArm);
  TouchLED.pressed(Autonomous);
  take_off();
}