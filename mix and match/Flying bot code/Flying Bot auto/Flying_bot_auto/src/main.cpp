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

// User defined function declarations
void Place_beam();
void Drop_down();
void Grab_Beam_up();
void R3F();
void Grab_then_up();
void AjpinBeam();
void Movemen_v_fb_d(double velocity, double text, double distance);
void Turn_h_v_m(double heading, double velocity, double momentum);

int Screen_precision = 0, Console_precision = 0, AIVision7_objectIndex = 0, LiftBeam = 0;

float Rup, Rdown, Lup, Ldown, R3Var, Kspeed;
bool pin, phase, isBusy, blink, boot, autobeam, stop, startgame, beamon, AjON, touched = true, beamoff = false;
double width, cenX;

// Starting angle
int _StartAngle = 26;

// ========== FUNCTIONS AFTER RENAME ==========

void Drop_down_beam()
{
  guide.extend(cylinder1);
  wait(100, msec);
  MotorBeam.setStopping(hold);
  beamon ? MotorBeam.setVelocity(100, percent) : MotorBeam.setVelocity(45, percent);
  MotorBeam.setMaxTorque(100, percent);
  MotorBeam.spin(forward);
  wait(1, seconds);
  while (MotorBeam.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorBeam.spin(forward);
  }
  // MotorBeam.spinFor(forward, 1000.0, degrees, false);
  // wait(1500, msec);
  MotorBeam.setStopping(brake);
  MotorBeam.setMaxTorque(0, percent);
  MotorBeam.stop();
  MotorBeam.stop(coast);
}

void Place_beam()
{
  MotorLeft.stop();
  MotorRight.stop();
  MotorBeam.setVelocity(65.0, percent);
  stop = true;
  guide.retract(cylinder1);
  MotorBeam.spinFor(forward, 60.0, degrees, true);
  Pneumatic_Pin_Beam.retract(cylinder2);
  guide.extend(cylinder1);
  MotorRight.stop();
  MotorLeft.stop();
  MotorLeft.setVelocity(85.0, percent);
  MotorRight.setVelocity(85.0, percent);
  MotorLeft.spinFor(forward, 130.0, degrees, false);
  MotorRight.spinFor(forward, 130.0, degrees, false);
  stop = false;
  Drop_down_beam();
}

void Grab_Beam_up()
{
  MotorBeam.setStopping(hold);
  MotorBeam.spinFor(reverse, 200.0, degrees, false);
  wait(0.8, seconds);
  guide.retract(cylinder1);
  wait(0.7, seconds);
  MotorBeam.stop();
  isBusy = false;
}

double ComputeTurnSpeed(double absErr, double maxSpeed)
{
  double minSpeed = maxSpeed * 0.2;
  double maxErrForScale = 95.0;

  double t = absErr / maxErrForScale;
  if (t > 1.0)
    t = 1.0;

  // curve: make small errors give much smaller speeds
  t = t * t; // square makes the curve “steeper” near 0

  double speed = minSpeed + (maxSpeed - minSpeed) * t;
  return speed;
}

// void Turn_h_v_m(double heading, double velocity, double momentum)
// {
//   while (heading != BrainInertial.heading())
//   {

//     // หมุนซ้าย
//     if (heading - momentum > BrainInertial.heading())
//     {
//       MotorLeft.setVelocity(velocity, percent);
//       MotorRight.setVelocity(velocity, percent);
//       MotorLeft.spin(forward);
//       MotorRight.spin(reverse);
//     }

//     // หมุนขวา
//     else if (heading + momentum < BrainInertial.heading())
//     {
//       MotorLeft.setVelocity(velocity, percent);
//       MotorRight.setVelocity(velocity, percent);
//       MotorLeft.spin(reverse);
//       MotorRight.spin(forward);
//     }

//     // ตรงแล้ว
//     else
//     {
//       MotorLeft.setStopping(brake);
//       MotorRight.setStopping(brake);
//       MotorLeft.stop();
//       MotorRight.stop();
//       while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
//       {
//         wait(10, msec);
//       }
//       MotorLeft.setStopping(coast);
//       MotorRight.setStopping(coast);
//       printf("%.2f\n", BrainInertial.heading());
//       if (0.0 == MotorLeft.velocity(percent))
//         break;
//     }
//     wait(20, msec);
//   }
//   wait(0.1, seconds);
// }

void TurnRight_h_v_m(double heading, double velocity, double momentum)
{
  while (true)
  {
    double current = BrainInertial.heading();
    double error = heading - current; // positive → need left turn
    double absErr = fabs(error);

    // ---- 1. Done / inside momentum zone ----
    if (absErr <= momentum)
    {
      MotorLeft.setStopping(brake);
      MotorRight.setStopping(brake);
      MotorLeft.stop();
      MotorRight.stop();

      while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
        wait(10, msec);

      printf("Final heading: %.2f\n", current);
      break;
    }

    // ---- 2. Slowdown system to prevent sliding ----
    double turnSpeed = ComputeTurnSpeed(absErr, velocity);
    // turnSpeed *= 0.9;
    if(turnSpeed < 10) turnSpeed = 10;

    MotorLeft.setVelocity(turnSpeed, percent);
    MotorRight.setVelocity(turnSpeed, percent);

    // ---- 3. Actual turn left ----
    MotorLeft.spin(forward);
    MotorRight.spin(reverse);

    wait(20, msec);
  }

  wait(0.1, seconds);
}

void TurnLeft_h_v_m(double heading, double velocity, double momentum)
{
  while (true)
  {
    double current = BrainInertial.heading();
    double error = heading - current; // negative → need right turn
    double absErr = fabs(error);

    // ---- 1. Done / inside momentum zone ----
    if (absErr <= momentum)
    {
      MotorLeft.setStopping(brake);
      MotorRight.setStopping(brake);
      MotorLeft.stop();
      MotorRight.stop();

      while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
        wait(10, msec);

      printf("Final heading: %.2f\n", current);
      break;
    }

    // ---- 2. Slowdown system to prevent sliding ----
    double turnSpeed = ComputeTurnSpeed(absErr, velocity);
    // turnSpeed *= 0.9;
    // if(turnSpeed < 10) turnSpeed = 10;

    MotorLeft.setVelocity(turnSpeed, percent);
    MotorRight.setVelocity(turnSpeed, percent);

    // ---- 3. Actual turn right ----
    MotorLeft.spin(reverse);
    MotorRight.spin(forward);

    wait(20, msec);
  }

  wait(0.1, seconds);
}

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
    isBusy = false;
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

void Drop_down_Grab_Up()
{
    stop = true;
    wait(0.1, seconds);
    stop = false;
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    MotorPin.spinToPosition(-50.0, degrees, false);
    wait(0.1, seconds);
    guide.retract(cylinder2);
    MotorPin.spinToPosition(-170.0, degrees, false);
    stop = true;
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
    wait(0.5, seconds);
    Pneumatic_Pin_Beam.extend(cylinder1);
    wait(0.3, seconds);
    MotorPin.resetPosition();
    MotorPin.spinToPosition(215.0, degrees, false);
    stop = false;
    wait(0.2, seconds);
    guide.extend(cylinder2);
    isBusy = false;
}

void R3F()
{
  Pneumatic_Pin_Beam.extend(cylinder2);
  guide.retract(cylinder2);
  MotorPin.spinFor(reverse, 700.0, degrees, false);
  wait(1500, msec);
  Pneumatic_Pin_Beam.retract(cylinder1);
  wait(0.1, seconds);
  MotorPin.setStopping(coast);
  MotorPin.spinFor(forward, 700.0, degrees, false);
  wait(1000, msec);
  MotorPin.stop(coast);
}

void Movemen_v_fb_d(double velocity, double FB, double distance)
{
  MotorLeft.setVelocity(velocity, percent);
  MotorRight.setVelocity(velocity, percent);
  if (FB == 0)
  {
    MotorLeft.spinFor(forward, (distance / 0.833), degrees, false);
    MotorRight.spinFor(forward, (distance / 0.833), degrees, true);
    MotorLeft.setStopping(brake);
    MotorRight.setStopping(brake);
    MotorLeft.stop();
    MotorRight.stop();
    while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
    {
      wait(10, msec);
    }
  }
  if (FB == 1)
  {
    MotorLeft.spinFor(reverse, (distance / 0.833), degrees, false);
    MotorRight.spinFor(reverse, (distance / 0.833), degrees, true);
    MotorLeft.setStopping(brake);
    MotorRight.setStopping(brake);
    MotorLeft.stop();
    MotorRight.stop();
    while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
    {
      wait(10, msec);
    }
  }
}

double ComputeApproachSpeed(double width, double maxSpeed)
{
  // Tune these three numbers for your robot & camera
  const double targetWidth =
      85.0; // how big the pin should look when you want to stop
  const double minDetectWidth =
      20.0; // typical width when it's "far but visible"

  double minSpeed = maxSpeed * 0.2; // don't go slower than 20% of max
  double maxErr = targetWidth - minDetectWidth;

  // Error = how much smaller than target the current width is
  double err = targetWidth - width;

  // Clamp error between 0 and maxErr
  if (err < 0)
    err = 0;
  if (err > maxErr)
    err = maxErr;

  // Normalize → 0..1
  double t = err / maxErr;

  // Curve: square to make it gentler near target
  t = t * t;

  // When far (err large) → t ~1 → speed ~maxSpeed
  // When close (err small) → t ~0 → speed ~minSpeed
  double speed = minSpeed + (maxSpeed - minSpeed) * t;
  return speed;
}

void Go_To_Pin(int Gopin)
{
  const double targetWidth = 120.0; // when object is this big, we stop
  const double maxSpeed = 100.0;    // max forward speed (tune if needed)

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
      double width = AIVision7.objects[bestIndex].width;

      // 2) Compute speed from width, like ComputeTurnSpeed
      double speed = ComputeApproachSpeed(width, maxSpeed);

      MotorLeft.setVelocity(speed, percent);
      MotorRight.setVelocity(speed, percent);
      MotorLeft.spin(forward);
      MotorRight.spin(forward);

      // 3) Close enough → brake and exit
      if (width >= targetWidth)
      {
        MotorLeft.stop(brake);
        MotorRight.stop(brake);

        // Optional: wait until really stopped
        while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
        {
          wait(10, msec);
        }
        MotorLeft.setStopping(coast);
        MotorRight.setStopping(coast);
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

void BeamMotorUp()
{
  while (true)
  {
    if (LiftBeam == 1)
    {
      MotorBeam.setMaxTorque(100, percent);
      MotorBeam.setVelocity(100, percent);
      MotorBeam.spinFor(reverse, 520, degrees, true);
      break;
    }
    if (LiftBeam == 2)
    {
      MotorBeam.setMaxTorque(100, percent);
      MotorBeam.setVelocity(100, percent);
      MotorBeam.spinFor(reverse, 100, degrees, true);
      break;
    }
    if (LiftBeam == 3)
    {
      MotorBeam.setMaxTorque(100, percent);
      MotorBeam.setVelocity(100, percent);
      MotorBeam.spinFor(reverse, 420, degrees, true);
      break;
    }
  }
}

void DriveUntil_fb_vel(int fb, int vel)
{
  MotorLeft.setVelocity(vel, percent);
  MotorRight.setVelocity(vel, percent);
  while (true)
  {
    if (fb == 0)
    {
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
      wait(500, msec);
      while (abs(MotorLeft.velocity(vex::velocityUnits::pct)) > 1)
      {
        MotorLeft.spin(reverse);
        MotorRight.spin(reverse);
        wait(250, msec);
      }
      wait(250, msec);
      MotorLeft.stop();
      MotorRight.stop();
      break;
    }
    if (fb == 1)
    {
      MotorLeft.spin(forward);
      MotorRight.spin(forward);
      wait(500, msec);
      while (MotorLeft.velocity(vex::velocityUnits::pct) > 1)
      {
        MotorLeft.spin(forward);
        MotorRight.spin(forward);
        wait(250, msec);
      }
      wait(250, msec);
      MotorLeft.stop();
      MotorRight.stop();
      break;
    }
  }
}

void Place_Standoff()
{
  MotorBeam.setMaxTorque(100, percent);
  // Changed Velocity Before 20
  if (!beamoff)
  {
    MotorBeam.setVelocity(100, percent);
    MotorBeam.spinFor(forward, 70, degrees, true);
    wait(0.3, seconds);
    Pneumatic_Pin_Beam.retract(cylinder2);
  }
  else
  {
    MotorBeam.setVelocity(45, percent);
    MotorBeam.spinFor(forward, 70, degrees, true);
    wait(0.4, seconds);
    Pneumatic_Pin_Beam.retract(cylinder2);
  }
  MotorBeam.spinFor(reverse, 60, degrees, false);
  Movemen_v_fb_d(100, 0, 200);
  MotorBeam.spinFor(forward, 535, degrees, false);
}

// ====================== WHEN STARTED ======================
int take_off()
{
  startgame = true;
  beamon = true;
  AjON = false;
  MotorPin.setPosition(0.0, degrees);
  MotorBeam.setPosition(0.0, degrees);
  MotorPin.setVelocity(80.0, percent);
  MotorPin.setMaxTorque(100.0, percent);
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
  MotorPin.setStopping(hold);
  MotorBeam.setStopping(coast);
  Pneumatic_Pin_Beam.retract(cylinder1);
  Pneumatic_Pin_Beam.retract(cylinder2);
  guide.extend(cylinder1);
  Pneumatic_Pin_Beam.pumpOn();
  TouchLED.setColor(red);
  Brain.Screen.setCursor(1, 1);
  guide.retract(cylinder2);
  Drop_down_beam();
  MotorBeam.spinFor(reverse, 300.0, degrees);
  wait(1, seconds);
  Pneumatic_Pin_Beam.extend(cylinder2);
  beamon = false;

  return 0;
}
void PrintCon()
{
  while (true)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("D2: %.2f", D2.objectDistance(mm));
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("D1: %.2f", D1.objectDistance(mm));
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("Heading: %.2f", BrainInertial.heading(degrees));
    Brain.Screen.setCursor(4, 1);
    wait(50, msec);
  }
}
double ComputeAlignSpeed(double absErr, double maxSpeed)
{
  double minSpeed = maxSpeed * 0.25; // don't go slower than 25%
  double maxErrForScale = 80.0;      // pixels; tune for your camera view

  double t = absErr / maxErrForScale;
  if (t > 1.0)
    t = 1.0;

  // Square curve: big error → near maxSpeed, small error → gentle
  t = t * t;

  double speed = minSpeed + (maxSpeed - minSpeed) * t;
  return speed;
}

void AjpinLeft(int targetIDLeft)
{
  // int count = 0;
  while (true)
  {
    TouchLED.setColor(orange);

    AIVision7.takeSnapshot(aivision::ALL_AIOBJS);

    double bestDist = 999;
    double offset = 0;

    for (int i = 0; i < AIVision7.objectCount; i++)
    {
      if (AIVision7.objects[i].id == targetIDLeft)
      {
        double cx = AIVision7.objects[i].centerX;
        double d = abs(cx - 80);

        if (d < bestDist)
        {
          bestDist = d;
          offset = cx - 80;
        }
      }
    }

    if (bestDist < 10)
    {
      MotorLeft.stop(brake);
      MotorRight.stop(brake);
      break;
    }
    else
    {
      MotorLeft.setVelocity(15, percent);
      MotorRight.setVelocity(15, percent);
      if (offset > 0)
      {
        MotorLeft.spin(forward);
        MotorRight.stop();
      }
      else if (offset < 0)
      {
        MotorLeft.stop();
        MotorRight.spin(forward);
      }
    }

    wait(10, msec);
    TouchLED.setColor(blue_green);
  }
}

void Ajpinright(int targetIDRight)
{
  int count = 0;
  while (true)
  {
    TouchLED.setColor(blue);

    AIVision7.takeSnapshot(aivision::ALL_AIOBJS);

    double bestDist = 999;
    double offset = 0;

    for (int i = 0; i < AIVision7.objectCount; i++)
    {
      if (AIVision7.objects[i].id == targetIDRight)
      {
        double cx = AIVision7.objects[i].centerX;
        double d = abs(cx - 210);

        if (d < bestDist)
        {
          bestDist = d;
          offset = cx - 210;
        }
      }
    }

    if (bestDist < 10)
    {
      count++;
      if (count == 2)
      {
        MotorLeft.stop(brake);
        MotorRight.stop(brake);
        break;
      }
    }
    else
    {
      MotorLeft.setVelocity(15, percent);
      MotorRight.setVelocity(15, percent);
      if (offset > 0)
      {
        MotorLeft.spin(forward);
        MotorRight.stop();
      }
      else if (offset < 0)
      {
        MotorLeft.stop();
        MotorRight.spin(forward);
      }
    }

    wait(10, msec);
    TouchLED.setColor(blue_green);
  }
}
void AjpinFind(int targetID)
{
  int count = 0;
  while (true)
  {
    TouchLED.setColor(red);

    AIVision7.takeSnapshot(aivision::ALL_AIOBJS);

    double bestDist = 999;
    double offset = 0;

    for (int i = 0; i < AIVision7.objectCount; i++)
    {
      if (AIVision7.objects[i].id == targetID)
      {
        double cx = AIVision7.objects[i].centerX;
        double d = abs(cx - 160);

        if (d < bestDist)
        {
          bestDist = d;
          offset = cx - 160;
        }
      }
    }

    if (bestDist < 30)
    {
      count++;
      if (count == 2)
      {
        MotorLeft.stop(brake);
        MotorRight.stop(brake);
        break;
      }
    }
    else
    {
      MotorLeft.setVelocity(30, percent);
      MotorRight.setVelocity(30, percent);

      if (offset > 0)
      {
        MotorLeft.spin(forward);
        MotorRight.stop();
      }
      else if (offset < 0)
      {
        MotorLeft.stop();
        MotorRight.spin(forward);
      }
    }

    wait(10, msec);
    TouchLED.setColor(blue_green);
  }
}

void FindBeam(int targetID)
{
  int count = 0;
  while (true)
  {
    TouchLED.setColor(purple);

    AIVision7.takeSnapshot(aivision::ALL_AIOBJS);

    double bestY = -1;
    double offset = 0;

    for (int i = 0; i < AIVision7.objectCount; i++)
    {
      if (AIVision7.objects[i].id == targetID)
      {
        double cy = AIVision7.objects[i].centerY;
        double d = abs(cy - 160);

        if (d < bestY)
        {
          bestY = d;
          offset = cy - 160;
        }
      }
    }

    if (bestY < 30)
    {
      count++;
      if (count == 2)
      {
        MotorLeft.stop(brake);
        MotorRight.stop(brake);
        break;
      }
    }
    else
    {
      MotorLeft.setVelocity(30, percent);
      MotorRight.setVelocity(30, percent);

      if (offset > 0)
      {
        MotorLeft.spin(forward);
        MotorRight.stop();
      }
      else if (offset < 0)
      {
        MotorLeft.stop();
        MotorRight.spin(forward);
      }
    }

    wait(10, msec);
    TouchLED.setColor(blue_green);
  }
}
// =================== TOUCH LED EVENT ===================
void onevent_TouchLED_pressed_0()
{
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
    // BrainInertial.isCalibrating();
    // while (BrainInertial.isCalibrating())
    // {
    //   task::sleep(50);
    //   TouchLED.setColor(black);
    // }
    TouchLED.setColor(orange);
    wait(0.2, seconds);

    touched = false;
  }
  else
  {

    // Initial
    TouchLED.setColor(purple);
    // BrainInertial.setHeading(0.0, degrees);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    startgame = false;
    Brain.Timer.reset();
    BrainInertial.setRotation(0.0, degrees);
    MotorBeam.spinFor(reverse, 220, degrees, false);

    // // Fist 2 Pins
    Movemen_v_fb_d(100, 0, 490.0);
    Movemen_v_fb_d(50, 0, 150.0);
    Pneumatic_Pin_Beam.extend(cylinder1);

    // // Init Second 2 Pins
    Movemen_v_fb_d(100, 1, 230.0);
    TurnLeft_h_v_m(270, 100.0, 3);

    Movemen_v_fb_d(100, 0, 350.0);
    TurnRight_h_v_m(296, 40, 3);
    Grab_then_up();

    // Grab Second 2 Pins
    Movemen_v_fb_d(100.0, 0, 490.0);
    Movemen_v_fb_d(20.0, 0, 160.0);
    Drop_down_Grab_Up();
    // Movemen_v_fb_d(100, 0, 80.0);
    TurnLeft_h_v_m(270, 100.0, 3);

    // // Place Pin on Standoff
    // Movemen_v_fb_d(100, 1, 200.0);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(reverse);
    MotorRight.spin(reverse);
    while (D1.objectDistance(inches) > 2.1 && D2.objectDistance(inches) > 2.1)
    {
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
    }
    wait(0.23, seconds);
    Place_Standoff();

    // U Beam
    MotorBeam.spinFor(forward, 535, degrees, false);
    TurnRight_h_v_m(90, 50, 3);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(reverse);
    MotorRight.spin(reverse);
    wait(0.5, seconds);
    while (MotorLeft.velocity(percent) != 0)
    {
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
    }
    R3F();
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(reverse);
    MotorRight.spin(reverse);
    while (MotorLeft.velocity(percent) != 0)
    {
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
    }

    // Place Beam on Standoff
    Movemen_v_fb_d(100, 0, 120);
    MotorBeam.spinFor(reverse, 200, degrees, false);
    TurnLeft_h_v_m(270, 50, 3);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spin(forward);
    MotorRight.spin(forward);
    wait(0.5, seconds);
    while (MotorLeft.velocity(percent) != 0)
    {
      MotorLeft.spin(forward);
      MotorRight.spin(forward);
    }
    wait(0.5, seconds);
    MotorLeft.setVelocity(60, percent);
    MotorRight.setVelocity(60, percent);
    MotorLeft.spin(reverse);
    MotorRight.spin(reverse);
    MotorBeam.setVelocity(100, percent);
    MotorBeam.spinFor(reverse, 320, degrees, false);
    wait(0.5, seconds);
    while (D1.objectDistance(inches) > 2.1 && D2.objectDistance(inches) > 2.1)
    {
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
    }
    wait(0.5, seconds);
    beamoff = true;
    Place_Standoff();
  }
}
int main()
{
  vexcodeInit();
  thread printco(PrintCon);
  TouchLED.pressed(onevent_TouchLED_pressed_0);
  take_off();
}