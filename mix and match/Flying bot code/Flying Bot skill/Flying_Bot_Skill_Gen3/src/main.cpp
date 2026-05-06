#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;

// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor MotorPinMotorA = motor(PORT3, false);
motor MotorPinMotorB = motor(PORT9, true);
motor_group MotorPin = motor_group(MotorPinMotorA, MotorPinMotorB);

motor MotorBeamMotorA = motor(PORT5, false);
motor MotorBeamMotorB = motor(PORT11, true);
motor_group MotorBeam = motor_group(MotorBeamMotorA, MotorBeamMotorB);

touchled TouchLED12 = touchled(PORT6);
pneumatic Pneumatic_Pin_Beam = pneumatic(PORT4);
motor MotorLeft = motor(PORT12, false);
motor MotorRight = motor(PORT10, true);
pneumatic guide = pneumatic(PORT8);
distance Distance1Left = distance(PORT1);
distance Distance2 = distance(PORT2);

// generating and setting random seed
void initializeRandomSeed()
{
  wait(100, msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  // Combine these values into a single integer
  int seed = int(
      xAxis + yAxis + zAxis);
  // Set the seed
  srand(seed);
}

void vexcodeInit()
{

  // Initializing random seed.
  initializeRandomSeed();
}

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

// User defined function
void Place_beam();
// User defined function
void Lup_Init();
// User defined function
void Drop_down_beam();
// User defined function
void Grab_Beam_up();
// User defined function
void Ldown_Init();
// User defined function
void Rdown_Init();
// User defined function
void Grab_then_up();
// User defined function
void Rup_Init();
// User defined function
void Drop_down();
// User defined function
void Drop_down_Grab_Up();

int Screen_precision = 0, Console_precision = 0;

float Rup, Rdown, Lup, Ldown, R3, Kspeed;

bool front_back, pin, phase, beam, isBusy, blink, boot, autobeam, stop, stand_off_up,
    Fbeam, start_stand_off_up, stakeONGo, MakeY, crash, Eup;

// User defined function
void Place_beam()
{
  MotorLeft.stop(hold);
  MotorRight.stop(hold);
  front_back = true;
  MotorBeam.setVelocity(65.0, percent);
  stop = true;
  guide.extend(cylinder1);
  wait(100, msec);
  MotorBeam.spinFor(forward, 100.0, degrees, true);
  Pneumatic_Pin_Beam.retract(cylinder2);
  MotorBeam.spinFor(reverse, 100.0, degrees, false);
  MotorRight.stop();
  MotorLeft.stop();
  MotorLeft.setVelocity(100.0, percent);
  MotorRight.setVelocity(100.0, percent);
  MotorLeft.spinFor(forward, 130.0, degrees, false);
  MotorRight.spinFor(forward, 130.0, degrees);
  stop = false;
  Drop_down_beam();
  // MotorBeam.spinFor(forward, 140.0, degrees, true);
  // MotorBeam.setPosition(0.0, degrees);
  // isBusy = false;
}

// User defined function
void Lup_Init()
{
  Brain.Timer.reset();
  Lup = Lup + 1.0;
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Drop_down_beam()
{
  stop = false;
  guide.extend(cylinder1);
  wait(100, msec);
  MotorBeam.setStopping(hold);
  beam ? MotorBeam.setVelocity(100, percent) : MotorBeam.setVelocity(45, percent);
  MotorBeam.spin(forward);
  MotorBeam.setMaxTorque(100, percent);
  wait(100, msec);
  while (MotorBeam.velocity(vex::velocityUnits::pct) > 1)
  {
    MotorBeam.spin(forward);
    wait(1, msec);
  }
  // MotorBeam.spinFor(forward, 1000.0, degrees, false);
  // wait(1500, msec);
  MotorBeam.setStopping(brake);
  MotorBeam.setMaxTorque(0, percent);
  MotorBeam.stop();
  isBusy = false;
}

// User defined function
void Grab_Beam_up()
{

  MotorBeam.spinFor(reverse, 240.00, degrees, false);
  wait(800, msec);
  guide.retract(cylinder1);
  // MotorBeam.stop();
  isBusy = false;
}

// User defined function
void Ldown_Init()
{
  Brain.Timer.reset();
  Ldown = Ldown + 1.0;
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Rdown_Init()
{
  Brain.Timer.reset();
  Rdown = Rdown + 1.0;
  MotorPin.setVelocity(50.0, percent);
  MotorPin.setMaxTorque(100.0, percent);
}

// User defined function
void Grab_then_up()
{
  MotorPin.resetPosition();
  MotorPin.setPosition(0.0, degrees);
  MotorPin.setStopping(hold);
  // MotorPin.spinFor(forward, 10.0, degrees, true);
  Pneumatic_Pin_Beam.extend(cylinder1);
  wait(0.3, seconds);
  // MotorPin.spinFor(forward, 180.0, degrees, false);
  MotorPin.spinToPosition(180.0, degrees, false);
  wait(0.2, seconds);
  guide.extend(cylinder2);
  isBusy = false;
}

// User defined function
void Rup_Init()
{
  Brain.Timer.reset();
  Rup = Rup + 1.0;
  MotorPin.setVelocity(100.0, percent);
  MotorPin.setMaxTorque(100.0, percent);
  MotorPin.setStopping(hold);
}

// User defined function
void Drop_down()
{
  MotorPin.resetPosition();
  MotorLeft.setStopping(hold);
  MotorRight.setStopping(hold);
  wait(0.35, seconds);
  // MotorPin.setStopping(coast);
  MotorPin.setVelocity(100, percent);
  // MotorPin.spinFor(reverse, 220.0, degrees, false);
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
  // MotorPin.spinFor(forward, 155.0, degrees, false);
  Pneumatic_Pin_Beam.retract(cylinder1);
  MotorPin.setStopping(coast);
  MotorPin.stop(coast);
  // wait(0.3, seconds);
  MotorLeft.setStopping(coast);
  MotorRight.setStopping(coast);
  MotorPin.setPosition(0.0, degrees);
}

// User defined function
void Drop_down_Grab_Up()
{
    stop = true;
    wait(0.1, seconds);
    stop = false;
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    // MotorPin.spinFor(reverse, 50.0, degrees, false);
    MotorPin.spinToPosition(-50.0, degrees, false);
    wait(0.1, seconds);
    guide.retract(cylinder2);
    MotorPin.spinToPosition(-170.0, degrees, false);
    // MotorPin.spinFor(reverse, 170.0, degrees, false);
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
    wait(0.35, seconds);
    Pneumatic_Pin_Beam.extend(cylinder1);
    wait(0.3, seconds);
    MotorPin.resetPosition();
    // MotorPin.spinFor(forward, 220.0, degrees, false);
    MotorPin.spinToPosition(215.0, degrees, false);
    stop = false;
    wait(0.2, seconds);
    guide.extend(cylinder2);
    isBusy = false;
}
// "when Controller ButtonLUp pressed" hat block
void LUp_pressed()
{
  wait(0.1, seconds);
  if (!isBusy)
  {
    Lup_Init();
    isBusy = true;
    if (Lup == 1.0)
    {
      front_back = false;
      Grab_Beam_up();
      Lup = 1.0;
      autobeam = true;
      MakeY = true;
    }
    else if (Lup == 2.0)
    {
      stop = true;
      beam = true;
      Place_beam();
      Lup = 0.0;
      autobeam = false;
      MakeY = false;
    }
    isBusy = false;
  }
}

// "when Controller ButtonLDown pressed" hat block
void LDown_pressed()
{
  Ldown_Init();
  wait(0.1, seconds);
  if (!isBusy && Lup == 1.0)
  {
    if (Ldown >= 1.0)
    {
      Drop_down_beam();
      Lup = 0.0;
      Ldown = 0.0;
      front_back = true;
      autobeam = false;
      stand_off_up = false;
    }
    isBusy = false;
  }
}
void Drive()
{
  if (!Controller.ButtonEUp.pressing())
  {
    if (front_back)
    {
      MotorRight.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) - (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * Kspeed), percent);
      MotorLeft.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) + (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * Kspeed), percent);
      MotorLeft.spin(forward);
      MotorRight.spin(forward);
    }
    else if (!front_back)
    {
      MotorRight.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) + (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * Kspeed), percent);
      MotorLeft.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) - (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * Kspeed), percent);
      MotorLeft.spin(reverse);
      MotorRight.spin(reverse);
    }
    if ((Controller.AxisA.position() > -5.0 and Controller.AxisA.position() < 5) and (Controller.AxisB.position() > -5.0 and Controller.AxisB.position() < 5))
    {
      MotorLeft.stop(coast);
      MotorRight.stop(coast);
    }
  }
}

// "when started" hat block
int seting()
{
  MotorPin.setPosition(0.0, degrees);
  // MotorBeam.setPosition(0.0, degrees);
  MotorPin.setVelocity(80.0, percent);
  MotorPin.setMaxTorque(100.0, percent);
  MotorBeam.setVelocity(100.0, percent);
  MotorBeam.setMaxTorque(100.0, percent);
  MotorPin.setStopping(hold);
  MotorBeam.setStopping(hold);
  MotorLeft.setStopping(coast);
  MotorRight.setStopping(coast);
  Pneumatic_Pin_Beam.retract(cylinder1);
  guide.retract(cylinder2);
  Pneumatic_Pin_Beam.retract(cylinder2);
  Pneumatic_Pin_Beam.pumpOn();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Flying Bot skill");
  pin = true;
  phase = true;
  beam = true;
  isBusy = false;
  Eup = true;
  Rup = 0.0;
  Rdown = 0.0;
  Lup = 1.0;
  Ldown = 1.0;
  R3 = 0.0;
  front_back = true;
  boot = false;
  Kspeed = 1.0;
  stop = false;
  autobeam = false;
  stand_off_up = false;
  Fbeam = true;
  start_stand_off_up = true;
  stakeONGo = false;
  crash = false;
  Drop_down();
  Drop_down_beam();
  MotorBeam.spinFor(reverse, 300.0, degrees);
  while (true)
  {
    if (!(Lup == 3.0 || stop))
    {
      Drive();
    }
    wait(25, msec);
  }

  return 0;
}

// "when Controller ButtonFDown pressed" hat block
void FDown_pressed()
{
  Brain.Timer.reset();
  wait(0.1, seconds);
  if (beam)
  {
    beam = false;
    Pneumatic_Pin_Beam.extend(cylinder2);
    blink = true;
    wait(0.2, seconds);
    blink = false;
  }
  else
  {
    beam = true;
    Pneumatic_Pin_Beam.retract(cylinder2);
  }
}

// "when Controller ButtonEDown pressed" hat block
void EDown_pressed()
{
  Brain.Timer.reset();
  wait(0.1, seconds);
  if (phase)
  {
    phase = false;
    guide.retract(cylinder1);
  }
  else
  {
    phase = true;
    guide.extend(cylinder1);
  }
}

// "when Controller ButtonFUp pressed" hat block
void FUp_pressed()
{
  Brain.Timer.reset();
  wait(0.1, seconds);
  if (pin)
  {
    pin = false;
    Pneumatic_Pin_Beam.extend(cylinder1);
  }
  else
  {
    pin = true;
    Pneumatic_Pin_Beam.retract(cylinder1);
  }
}

// "when started" hat block
int LED_controller()
{
  while (true)
  {
    if (front_back)
    {
      TouchLED12.setColor(purple);
      if (blink)
      {
        TouchLED12.setColor(green);
        wait(500, msec);
      }
      else
      {
        if (!boot)
        {
          TouchLED12.setColor(purple);
          wait(0.02, seconds);
          TouchLED12.setColor(colorType::none);
        }
      }
    }
    else
    {
      TouchLED12.setColor(orange);
      if (blink)
      {
        TouchLED12.setColor(green);
        wait(500, msec);
      }
      else
      {
        if (!boot)
        {
          TouchLED12.setColor(orange);
          wait(0.02, seconds);
          TouchLED12.setColor(colorType::none);
        }
      }
    }
    wait(20, msec);
  }
  return 0;
}

// "when Controller ButtonRUp pressed" hat block
void RUp_pressed()
{
  Rup_Init();
  if (!isBusy)
  {
    isBusy = true;
    if (Rup == 1.0)
    {
      Grab_then_up();
      pin = false;
    }
    else if (Rup == 2.0)
    {
      Drop_down_Grab_Up();
      Rup = 1.0;
      pin = false;
    }
    else
    {
      Rup = 1.0;
    }
    isBusy = false;
  }
}

// "when started" hat block
int Time()
{
  Brain.Timer.reset();
  Brain.Screen.setFont(mono20);
  while ((Brain.Timer.value() < 300.0))
  {
    Brain.Screen.setCursor(1, 1);
    wait(1, msec);
  }
  Brain.programStop();
  return 0;
}

// "when Controller ButtonRDown pressed" hat block
void RDown_pressed()
{
  Rdown_Init();
  if (!isBusy && Rup == 1.0)
  {
    isBusy = true;
    if (Rdown >= 1.0)
    {
      Drop_down();
      Rup = 0.0;
      Rdown = 0.0;
      crash = false;
      pin = true;
    }
    isBusy = false;
  }
}

// "when Controller ButtonEUp pressed" hat block
void EUp_pressed()
{
  if (Eup)
  {
    MotorPin.spinFor(forward, 95.0, degrees, true);
    Eup = false;
  }
  else
  {
    Brain.Timer.reset();
    stop = true;
    Pneumatic_Pin_Beam.extend(cylinder1);
    MotorPin.spinFor(forward, 80.0, degrees);
    MotorLeft.setVelocity(100, percent);
    MotorRight.setVelocity(100, percent);
    MotorLeft.spinFor(reverse, 200.0, degrees, false);
    MotorRight.spinFor(reverse, 200.0, degrees);
    guide.extend(cylinder2);
    stop = false;
    //----
    MotorPin.resetPosition();
    MotorLeft.setStopping(hold);
    MotorRight.setStopping(hold);
    wait(0.35, seconds);
    // MotorPin.setStopping(coast);
    MotorPin.setVelocity(100, percent);
    // MotorPin.spinFor(reverse, 220.0, degrees, false);
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
    // MotorPin.spinFor(forward, 155.0, degrees, false);
    MotorPin.setStopping(coast);
    MotorPin.stop(coast);
    // wait(0.3, seconds);
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    wait(0.5, seconds);
    Pneumatic_Pin_Beam.retract(cylinder1);
    MotorPin.setPosition(0.0, degrees);
    //----
    Rup = 0.0;
    Rdown = 0.0;
    pin = true;
    Eup = true;
  }
}

// "when Controller AxisD changed" hat block
// void ControllerDChanged()
// {
//   if (Controller.AxisD.position() >= 90.0)
//   {
//     guide.retract(cylinder2);
//   }
//   else if (Controller.AxisD.position() <= -90.0)
//   {
//     guide.extend(cylinder2);
//   }
// }

void ControllerDChanged()
{
  if (!isBusy)
  {
    if (Controller.AxisD.position() >= 90.0)
    {
      guide.retract(cylinder2);
      MotorPin.resetPosition();
      MotorPin.setVelocity(45, percent);
      wait(350, msec);
      MotorPin.spinFor(reverse, 122.0, degrees, true);
      // MotorPin.spinFor(115.0, degrees, true);
      MotorPin.setVelocity(100, percent);
      stakeONGo = true;
      Eup = false;
    }
  }
}

// "when Controller AxisC changed" hat block
void ControllerCChanged()
{
  MotorBeam.setMaxTorque(100, percent);
  MotorBeam.setVelocity(100.0, percent);

  if (Controller.AxisC.position() > 90.0)
  {
    if (stand_off_up)
    {
      stop = true;
      MotorLeft.stop();
      MotorRight.stop();
      front_back = true;
      if (start_stand_off_up)
      {
        MotorBeam.setVelocity(100.0, percent);
        MotorBeam.spinFor(forward, 90.00, degrees);
        Pneumatic_Pin_Beam.retract(cylinder2);
        // use for NO.3 wait 250 msec
        wait(250, msec);
        MotorBeam.spinFor(reverse, 100.00, degrees, false);
        start_stand_off_up = false;
      }
      else
      {
        MotorBeam.setVelocity(45.0, percent);
        MotorBeam.spinFor(forward, 60.00, degrees);
        Pneumatic_Pin_Beam.retract(cylinder2);
        MotorRight.stop();
        MotorLeft.stop();
        MotorBeam.spinFor(reverse, 70.00, degrees, false);
      }
      MotorLeft.setVelocity(100.0, percent);
      MotorRight.setVelocity(100.0, percent);
      MotorLeft.spinFor(forward, 350.0, degrees, false);
      MotorRight.spinFor(forward, 350.0, degrees, false);
      wait(0.6, seconds);
      beam = true;
      Drop_down_beam();
      stop = false;
      stand_off_up = false;
      Lup = 0.0;
    }
  }
  else if (Controller.AxisC.position() < -90.0)
  {
    if (!stand_off_up)
    {
      if (start_stand_off_up)
      {
        MotorBeam.spinFor(reverse, 210.0, degrees, false);
        stand_off_up = true;
        Lup = 1.0;
      }
      else if (!MakeY)
      {
        MotorBeam.spinFor(reverse, 460.0, degrees, false);
        stand_off_up = true;
        Lup = 1.0;
      }
    }
  }
}

// "when Controller ButtonR3 pressed" hat block
void ControllerButtonR3_pressed()
{
    Brain.Timer.reset();
    MotorPin.setVelocity(100.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    if (!isBusy && Rup == 1.0)
    {
        isBusy = true;
        if (Rup == 1.0)
        {
            guide.retract(cylinder2);
            MotorPin.spinFor(forward, 600.0, degrees, false);
            wait(0.8, seconds);
            MotorLeft.stop(coast);
            MotorRight.stop(coast);
            wait(0.35, seconds);
            Pneumatic_Pin_Beam.retract(cylinder1);
            // front_back = false;
            isBusy = false;
            wait(0.4, seconds);
            MotorPin.spinFor(reverse, 635.0, degrees, false);
            wait(0.4, seconds);
            // Grab_Beam_up();
            Drop_down();
            MotorPin.setStopping(coast);
            MotorPin.stop();
            // Lup = 1.0;
            Rup = 0.0;
            autobeam = true;
            pin = true;
            wait(0.1, seconds);
        }
    }
}

void ControllerButtonL3_pressed()
{

  stop = true;
  pin = true;
  Pneumatic_Pin_Beam.retract(cylinder1);
  MotorLeft.setVelocity(60, percent);
  MotorRight.setVelocity(60, percent);
  MotorLeft.spinFor(reverse, 50.0, degrees, false);
  MotorRight.spinFor(reverse, 50.0, degrees);
  MotorPin.setVelocity(100, percent);
  MotorPin.spinFor(reverse, 250, degrees);
  guide.retract(cylinder2);
  MotorLeft.setVelocity(45, percent);
  MotorRight.setVelocity(45, percent);
  MotorLeft.spinFor(forward, 60.0, degrees, false);
  MotorRight.spinFor(forward, 60.0, degrees);
  MotorLeft.spinFor(reverse, 200.0, degrees, false);
  MotorRight.spinFor(reverse, 200.0, degrees);
  Drop_down();
  stop = false;
}

int main()
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // register event handlers
  Controller.ButtonLUp.pressed(LUp_pressed);
  Controller.ButtonLDown.pressed(LDown_pressed);
  Controller.ButtonFDown.pressed(FDown_pressed);
  Controller.ButtonEDown.pressed(EDown_pressed);
  Controller.ButtonFUp.pressed(FUp_pressed);
  Controller.ButtonRUp.pressed(RUp_pressed);
  Controller.ButtonRDown.pressed(RDown_pressed);
  Controller.ButtonEUp.pressed(EUp_pressed);
  Controller.AxisD.changed(ControllerDChanged);
  Controller.AxisC.changed(ControllerCChanged);
  Controller.ButtonR3.pressed(ControllerButtonR3_pressed);
  Controller.ButtonL3.pressed(ControllerButtonL3_pressed);

  thread ws1_Led(LED_controller);
  thread ws2_time(Time);
  thread ws3_controller(seting);
  // vex::task ws1(LED_controller);
  // vex::task ws2(Time);
}