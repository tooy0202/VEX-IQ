/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Robot:        Teamwork_red                                              */
/*    Created:      24/1/2026, 08:59:47 AM                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;

// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor MotorPinMotorA = motor(PORT6, false);
motor MotorPinMotorB = motor(PORT12, true);
motor_group MotorPin = motor_group(MotorPinMotorA, MotorPinMotorB);

motor MotorBeamMotorA = motor(PORT8, true);
motor MotorBeamMotorB = motor(PORT9, false);
motor_group MotorBeam = motor_group(MotorBeamMotorA, MotorBeamMotorB);

pneumatic Pneumatic_font = pneumatic(PORT2);
pneumatic Pneumatic_back = pneumatic(PORT3);

distance Distance = distance(PORT4);

touchled TouchLED = touchled(PORT10);

motor MotorLeft = motor(PORT5, false);
motor MotorRight = motor(PORT11, true);

//================ ENUM =================

enum class DriveDirection
{
    FORWARD,
    BACKWARD
};

enum class BeamState
{
    ON,
    OFF
};

enum class PinState
{
    ON,
    OFF
};

enum class ButtonStage
{
    IDLE,
    STEP1,
    STEP2
};

//================ STRUCT =================

struct RobotState
{
    DriveDirection driveDir;
    BeamState beamState;
    PinState pinState;

    bool stop = false;
    bool isBusy = false;
    // bool IDoItNow = false;
    bool MakeY = false;
    bool Eup = false;
    bool EDown = false;
    bool Go = false;
    bool NewDowm = false;

    // double nowTime = 0;
    double Kspeed = 1.0;
    // double Maxturn = 45.0;
    // Robot NO2 = 90.0
    // Robot NO3 = 95.0
    // double goalArm = 90.0;
};

// controller state
struct ControllerState
{
    ButtonStage Lup = ButtonStage::IDLE;
    ButtonStage Ldown = ButtonStage::IDLE;
    ButtonStage Rup = ButtonStage::IDLE;
    ButtonStage Rdown = ButtonStage::IDLE;
};

//================ GLOBAL OBJECT =================

RobotState robot;
ControllerState controllerState;

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
bool Endgame = false;
bool Rdown_on = false;
// double turnSpeed = 0.0;
// double turnStep = 10.0;         // อัตราเร่งต่อรอบ (ปรับได้)
// double turnMax = robot.Maxturn; // จำกัดความเร็วเลี้ยวสูงสุด
// double ControTurn = 0.45;

#pragma endregion VEXcode Generated Robot Configuration

// User defined function
void Spin_Robot();
void Place_beam();
void Grab_Beam_up();
void Lup_Init();
void Drop_down_beam();
void Ldown_Init();
void Rdown_Init();
void Grab_then_up();
void Rup_Init();
void Drop_down();
void Drop_down_Grab_Up();
void Extend_Pneumatic_Beam();
void Retract_Pneumatic_Beam();
void Extend_joint_pin();
void Retract_joint_pin();
void Retract_2Pneumatic_Pin();
void Extend_2Pneumatic_Pin();
void Ajpin();
void EDown_pressde();
void RDown_pressed();

int Screen_precision = 0, Console_precision = 0;

// User defined function
void Place_beam()
{
    robot.stop = true;
    robot.driveDir = DriveDirection::FORWARD;
    MotorBeam.setStopping(brake);
    MotorBeam.stop();
    // if (robot.MakeY)
    // {
    MotorBeam.setVelocity(50.0, percent);
    MotorBeam.spinFor(forward, 120.0, degrees, true);
    // }
    // else
    // {
    //     MotorBeam.setVelocity(70.0, percent);
    // MotorLeft.spin(reverse, 20, pct);
    // MotorRight.spin(reverse, 20, pct);
    // wait(0.1, seconds);
    //     MotorLeft.setStopping(hold);
    //     MotorRight.setStopping(hold);
    //     MotorLeft.stop();
    //     MotorRight.stop();
    //     wait(0.2, seconds);
    //     MotorBeam.spinFor(forward, 60.0, degrees, true);
    //     wait(0.2, seconds);
    // }
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    wait(100, msec);
    MotorLeft.setVelocity(85.0, percent);
    MotorRight.setVelocity(85.0, percent);
    MotorBeam.spinFor(reverse, 15.0, degrees, false);
    MotorLeft.spinFor(forward, 500.0, degrees, false);
    MotorRight.spinFor(forward, 500.0, degrees, false);
    wait(0.4, seconds);
    Drop_down_beam();
    robot.stop = false;
    robot.beamState = BeamState::OFF;
    robot.isBusy = false;
}

// User defined function
void Grab_Beam_up()
{
    MotorBeam.spinFor(reverse, 265.0, degrees, false);
    robot.isBusy = false;
}

// User defined function
void Lup_Init()
{
    Brain.Timer.reset();
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Drop_down_beam()
{
    controllerState.Lup = ButtonStage::IDLE;
    robot.stop = false;
    MotorBeam.setStopping(hold);
    robot.beamState == BeamState::OFF ? MotorBeam.setVelocity(100, percent) : MotorBeam.setVelocity(45, percent);
    MotorBeam.spin(forward);
    MotorBeam.setMaxTorque(100, percent);
    wait(100, msec);
    while (MotorBeam.velocity(vex::velocityUnits::pct) != 0)
    {
        MotorBeam.spin(forward);
        wait(10, msec);
    }
    MotorBeam.setStopping(brake);
    MotorBeam.setMaxTorque(0, percent);
    MotorBeam.stop();
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    robot.isBusy = false;
}

// User defined function
void Ldown_Init()
{
    Brain.Timer.reset();
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Rdown_Init()
{
    Brain.Timer.reset();
    MotorLeft.setStopping(hold);
    MotorRight.setStopping(hold);
    MotorPin.setVelocity(50.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
}

// User defined function
void Grab_then_up()
{
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    Extend_2Pneumatic_Pin();
    wait(0.1, seconds);
    MotorPin.spinToPosition(190.0, degrees, false);
    wait(0.3, seconds);
    Extend_joint_pin();
}
// User defined function
void Rup_Init()
{
    Brain.Timer.reset();
    MotorPin.setVelocity(100.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorPin.setStopping(hold);
}

// User defined function
void Drop_down()
{
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    MotorPin.spin(reverse);
    wait(0.1, seconds);
    Retract_joint_pin();
    MotorPin.setVelocity(100, pct);
    MotorPin.spin(reverse);
    wait(0.15, sec);
    while (MotorPin.velocity(vex::velocityUnits::pct) > fabs(1))
    {
        MotorPin.spin(reverse);
        wait(1, msec);
    }
    Retract_2Pneumatic_Pin();
    wait(100, msec);
    MotorPin.setStopping(coast);
    MotorPin.stop();
    robot.pinState = PinState::OFF;
    robot.isBusy = false;
    robot.Eup = true;
    controllerState.Rup = ButtonStage::IDLE;
    robot.NewDowm = true;
    // MotorPin.setPosition(0.0, degrees);
    // MotorPin.spinFor(forward, 70, degrees, true);
}

// User defined function
void Drop_down_Grab_Up()
{
    // robot.IDoItNow = true;
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    MotorPin.setVelocity(80.0, percent);
    MotorPin.spin(reverse);
    wait(0.15, seconds);
    while (MotorPin.velocity(vex::velocityUnits::pct) > 1)
    {
        MotorPin.spin(reverse);
        wait(1, msec);
    }
    robot.stop = true;
    MotorLeft.setStopping(brake);
    MotorRight.setStopping(brake);
    MotorLeft.setVelocity(100.0, percent);
    MotorRight.setVelocity(100.0, percent);
    MotorLeft.stop();
    MotorRight.stop();
    wait(0.1, seconds);
    Retract_joint_pin();
    Retract_2Pneumatic_Pin();
    MotorPin.spin(reverse);
    robot.stop = true;
    wait(0.15, seconds);
    MotorLeft.spinFor(forward, 300.0, degrees, false);
    MotorRight.spinFor(forward, 300.0, degrees, false);
    wait(0.2, seconds);
    Extend_2Pneumatic_Pin();
    wait(0.1, seconds);
    MotorPin.setStopping(hold);
    MotorPin.stop();
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    MotorLeft.stop();
    MotorRight.stop();
    MotorPin.setVelocity(100, percent);
    MotorPin.setMaxTorque(100, pct);
    MotorPin.resetPosition();
    if (robot.Go == true)
    {
        if (controllerState.Rup == ButtonStage::STEP2)
        {
            MotorPinMotorB.setVelocity(100, pct);
            MotorPinMotorA.setVelocity(100, pct);
            MotorLeft.spin(forward, 50, pct);
            MotorRight.spin(forward, 50, pct);
            MotorPinMotorB.spin(forward);
            MotorPinMotorA.spin(forward);
            wait(400, msec);
            MotorPinMotorB.stop(hold);
            wait(35, msec);
            MotorPinMotorA.stop(hold);
            Rdown_on = true;
        }
        else
        {
            MotorPin.spinToPosition(127.0, degrees, false);
            Rdown_on = true;
        }
    }
    else if (robot.EDown == true)
    {
        // MotorPin.spinToPosition(130.0, degrees, false);
        //no4 140
        MotorPin.spinToPosition(140.0, degrees, false);
        // blue
        // Pneumatic_font.retract(cylinder2);
        // red
        Pneumatic_back.retract(cylinder1);
        Endgame = true;
    }
    else
    {
        MotorPin.spinToPosition(235.0, degrees, false);
    }
    robot.stop = false;
    wait(0.3, seconds);
    if (robot.Go == false && robot.EDown == false)
    {
        Extend_joint_pin();
    }
    else
    {
        robot.EDown = false;
    }
    robot.isBusy = false;
}
// User defined function
void Spin_Robot()
{
    robot.stop = true;
    MotorLeft.stop();
    MotorRight.stop();
    MotorPin.setStopping(coast);
    robot.driveDir = DriveDirection::BACKWARD;
    if (robot.EDown == true)
    {
        /* pass */
    }
    else
    {
        if (controllerState.Lup == ButtonStage::STEP1 || controllerState.Lup == ButtonStage::STEP2)
        {
            Drop_down();
            Retract_2Pneumatic_Pin();
            controllerState.Rup = ButtonStage::IDLE;
            controllerState.Rdown = ButtonStage::IDLE;
        }
        else
        {
            MotorPin.setStopping(coast);
        }
    }
    MotorRight.setStopping(hold);
    MotorLeft.setStopping(hold);
    MotorLeft.setVelocity(100.0, percent);
    MotorRight.setVelocity(100.0, percent);
    MotorLeft.spinFor(reverse, 50.0, degrees, false);
    MotorRight.spinFor(reverse, 50.0, degrees, true);
    MotorLeft.setVelocity(100.0, percent);
    MotorRight.setVelocity(100.0, percent);
    MotorLeft.spinFor(forward, 310.0, degrees, false);
    MotorRight.spinFor(reverse, 310.0, degrees, true);
    MotorRight.setStopping(coast);
    MotorLeft.setStopping(coast);
    MotorLeft.stop();
    MotorRight.stop();
    robot.pinState = PinState::OFF;
}

// void standoff()
// {
//     if (robot.isBusy == false)
//     {
//         robot.isBusy = true;
//         Brain.Timer.reset();
//         MotorBeam.setMaxTorque(100, percent);
//         MotorBeam.setVelocity(100.0, percent);
//         if (Controller.AxisC.position() > 90.0)
//         {
//             robot.stop = true;
//             wait(0.1, sec);
//             robot.stop = true;
//             MotorLeft.spin(forward, 50.0, percent);
//             MotorRight.spin(forward, 50.0, percent);
//             // MotorBeam.setStopping(coast);
//             // MotorBeam.stop();
//             Retract_2Pneumatic_Pin();
//             MotorPin.spin(reverse);
//             wait(400, msec);
//             MotorBeam.spinFor(reverse, 900.0, degrees, false);
//             MotorPin.stop(coast);
//             robot.pinState = PinState::OFF;
//             Retract_joint_pin();
//             MotorBeam.setStopping(hold);
//             Spin_Robot();
//             MotorPin.stop();
//             controllerState.Lup = ButtonStage::STEP1;
//             controllerState.Rup = ButtonStage::IDLE;
//             robot.MakeY = false;
//             robot.stop = false;
//             // Brain.Screen.clearLine(3, 1);
//             // robot.nowTime = Brain.Timer.value();
//             // Brain.Screen.setCursor(3, 1);
//             // Brain.Screen.print("ConC_Done: %.2f", robot.nowTime);
//         }
//         else if (Controller.AxisC.position() < -90.0)
//         {
//             MotorPin.setVelocity(100, pct);
//             MotorPin.spinToPosition(320, degrees, false);
//             wait(100, msec);
//             Extend_joint_pin();
//             // Brain.Screen.clearLine(3, 1);
//             // robot.nowTime = Brain.Timer.value();
//             // Brain.Screen.setCursor(3, 1);
//             // Brain.Screen.print("ConC_Done: %.2f", robot.nowTime);
//         }
//         robot.isBusy = false;
//     }
// }

void Ajpin()
{
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    MotorLeft.stop();
    MotorRight.stop();
    wait(50, msec);
    TouchLED.setColor(green);
    MotorLeft.spin(reverse, 100, pct);
    MotorRight.spin(reverse, 100, pct);
    wait(0.25, seconds);
    // while (MotorLeft.velocity(pct) != 0 && MotorRight.velocity(pct) != 0)
    // {
    //     wait(100, msec);
    // }
    // wait(0.3, seconds);
    // MotorLeft.stop();
    // MotorRight.stop();
    // wait(0.1, seconds);
    MotorLeft.setVelocity(45, percent);
    MotorRight.setVelocity(45, percent);
    MotorRight.spinFor(forward, 186.5, degrees, false);
    MotorLeft.spinFor(forward, 186.5, degrees, true);
    wait(200, msec);
}

void Pin_On_the_sidelines()
{
    if (robot.Eup)
    {
        MotorPin.resetPosition();
        MotorPin.spinFor(100.0, degrees, false);
        robot.Eup = false;
    }
    else
    {
        Brain.Timer.reset();
        robot.stop = true;
        MotorLeft.setVelocity(100, percent);
        MotorRight.setVelocity(100, percent);
        MotorPin.resetPosition();
        Extend_2Pneumatic_Pin();
        wait(0.2, seconds);
        MotorLeft.spinFor(reverse, 130.0, degrees, false);
        MotorRight.spinFor(reverse, 130.0, degrees, false);
        MotorPin.spinToPosition(80.0, degrees, false);
        wait(0.2, seconds);
        robot.stop = false;
        //----
        wait(0.2, seconds);
        MotorPin.setVelocity(100, percent);
        MotorPin.setMaxTorque(1, pct);
        MotorPin.spin(reverse);
        wait(250, msec);
        robot.isBusy = true;
        while (MotorPin.velocity(vex::velocityUnits::pct) < -1)
        {
            MotorPin.spin(reverse);
            wait(1, msec);
        }
        MotorPin.setStopping(coast);
        MotorPin.stop(coast);
        MotorLeft.setStopping(coast);
        MotorRight.setStopping(coast);
        MotorPin.setMaxTorque(100, pct);
        wait(0.1, sec);
        MotorPin.resetPosition();
        MotorPin.setPosition(0.0, degrees);
        MotorPin.setStopping(hold);
        Extend_2Pneumatic_Pin();
        wait(0.1, seconds);
        MotorPin.spinToPosition(175.0, degrees, false);
        wait(0.3, seconds);
        Extend_joint_pin();
        //----
        controllerState.Rup = ButtonStage::STEP1;
        controllerState.Rdown = ButtonStage::IDLE;
        robot.pinState = PinState::OFF;
        robot.isBusy = false;
        robot.Eup = true;
    }
}

// void Pin_on_go()
// {
//     MotorPin.setVelocity(60, percent);
//     if (robot.isBusy == false)
//     {
//         if (Controller.AxisD.position() >= 90.0)
//         {
//             if (robot.Go == true)
//             {
//                 MotorPin.resetPosition();
//                 MotorPin.spinFor(reverse, 20.0, degrees, false);
//             }
//             else
//             {
//                 robot.stop = true;
//                 MotorLeft.stop(coast);
//                 MotorRight.stop(coast);
//                 // robot.Eup = true;
//                 Pneumatic_font.retract(cylinder1);
//                 Retract_joint_pin();
//                 // wait(350, msec);
//                 // Robot NO2 = 90.0
//                 // Robot NO3 = 95.0
//                 robot.Go = true;
//                 MotorLeft.spin(forward, 20, percent);
//                 MotorRight.spin(forward, 20, percent);
//                 if (MotorPin.position(deg) <= 190)
//                 {
//                     wait(0.1, seconds);
//                     MotorPin.spinFor(reverse, 95.0, degrees, true);
//                 }
//                 else
//                 {
//                     wait(0.1, seconds);
//                     MotorPin.spinFor(reverse, 100.0, degrees, true);
//                 }
//                 robot.stop = false;
//             }
//         }
//         else if (Controller.AxisD.position() <= -90.0)
//         {
//             if (robot.Go == true)
//             {
//                 MotorPin.resetPosition();
//                 MotorPin.spinFor(forward, 20.0, degrees, false);
//             }
//         }
//     }
//     MotorPin.setVelocity(100, percent);
//     robot.stop = false;
//     // Brain.Screen.clearLine(3, 1);
//     // robot.nowTime = Brain.Timer.value();
//     // Brain.Screen.setCursor(3, 1);
//     // Brain.Screen.print("ConD_Done: %.2f", robot.nowTime);
// }

void flip()
{
    Brain.Timer.reset();
    MotorPin.setVelocity(100.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    // robot.Maxturn = 80.0;
    // ControTurn = 0.8;
    // if (robot.isBusy == false && (controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2))
    // {
    robot.isBusy = true;
    MotorBeam.spin(forward);
    robot.isBusy = true;
    // MotorPin.resetPosition();
    // MotorPin.setPosition(0.0, degrees);
    // MotorBeam.stop(coast);
    Retract_joint_pin();
    Retract_joint_pin();
    Retract_joint_pin();
    Retract_joint_pin();
    Retract_joint_pin();
    wait(100, msec);
    // MotorPin.spinFor(forward, 700.0, degrees, false);
    // MotorPin.spin(forward);
    //---ลอง
    MotorPin.spinFor(900 - (MotorPin.position(degrees)), degrees, false);
    // wait(0.55, seconds);
    // vexDelay(550);
    while (MotorPin.velocity(vex::velocityUnits::pct) > 5 || MotorPin.position(degrees) < 750)
    {
        continue;
    }
    // MotorBeam.spin(forward);
    MotorPin.stop(coast);
    //*--
    // wait(1000, msec);
    // MotorPin.setMaxTorque(100, percent);
    // MotorPin.spin(forward);
    // wait(700, msec);
    // wait(0.1, seconds);
    // if (MotorPin.position(vex::rotationUnits::deg) < 650)
    // {
    //     // MotorPin.spin(forward);
    //     MotorPin.stop(coast);
    //     wait(100, msec);
    // }
    Retract_2Pneumatic_Pin();
    Retract_2Pneumatic_Pin();
    Retract_2Pneumatic_Pin();
    Retract_2Pneumatic_Pin();
    Retract_2Pneumatic_Pin();
    // Pneumatic_font.retract(cylinder1);
    // wait(0.1, seconds);
    // Pneumatic_font.extend(cylinder1);
    // MotorBeam.stop(brake);
    MotorPin.setMaxTorque(100, percent);
    MotorPin.setVelocity(100, percent);
    MotorPin.spinFor(reverse, 1500.0, degrees, false);
    // robot.Maxturn = 40.0;
    // ControTurn = 0.40;
    wait(1, seconds);
    Drop_down();

    controllerState.Rup = ButtonStage::IDLE;
    robot.pinState = PinState::OFF;
    wait(0.1, seconds);
    // robot.isBusy = false;

    robot.stop = false;
    MotorBeam.spinFor(reverse, 100.0, degrees, false);
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("R3_Done: %.2f", robot.nowTime);
    // }
}

// "when Controller ButtonLUp pressed" hat block
void LUp_pressed()
{
    if (robot.isBusy == false)
    {
        Lup_Init();
        robot.isBusy = true;
        if (controllerState.Lup == ButtonStage::IDLE)
        {
            robot.stop = true;
            robot.driveDir = DriveDirection::BACKWARD;
            if (controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2)
                Drop_down();
            Retract_2Pneumatic_Pin();
            Grab_Beam_up();
            Spin_Robot();
            robot.stop = false;
            controllerState.Lup = ButtonStage::STEP1;
            robot.MakeY = true;
        }
        else if (controllerState.Lup == ButtonStage::STEP1)
        {
            wait(0.2, sec);
            robot.stop = true;
            robot.beamState = BeamState::ON;
            if (robot.MakeY)
            {
                Ajpin();
            }
            Place_beam();
            controllerState.Lup = ButtonStage::IDLE;
            robot.MakeY = false;
        }
        robot.isBusy = false;
    }
}

// "when Controller ButtonLDown pressed" hat block
void LDown_pressed()
{
    if (robot.isBusy == false && controllerState.Lup == ButtonStage::STEP1)
    {
        Ldown_Init();
        Drop_down_beam();
        controllerState.Ldown = ButtonStage::IDLE;
        robot.driveDir = DriveDirection::FORWARD;
        robot.isBusy = false;
    }
}

void Drive()
{
    // if (!Controller.ButtonEUp.pressing())
    // {
    // double speedTurnAF = 0.0;
    double turnSpeed = (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001);
    double forwardSpeed = (Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001);
    // turnSpeed *= ControTurn;
    // if (fabs(Controller.AxisA.position()) > 80.0)
    // {
    //     turnSpeed += turnStep * (Controller.AxisB.position() / 100.0);
    //     if (turnSpeed > turnMax)
    //         turnSpeed = turnMax;
    //     if (turnSpeed < -turnMax)
    //         turnSpeed = -turnMax;
    // }

    if (robot.driveDir == DriveDirection::FORWARD)
    {
        // red puple blue green
        TouchLED.setColor(purple);
        MotorRight.setVelocity((forwardSpeed - turnSpeed) * robot.Kspeed, percent);
        MotorLeft.setVelocity((forwardSpeed + turnSpeed) * robot.Kspeed, percent);
        MotorRight.spin(forward);
        MotorLeft.spin(forward);
    }
    else if (robot.driveDir == DriveDirection::BACKWARD)
    {
        TouchLED.setColor(orange);
        // double forwardSpeed = (Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001);

        MotorRight.setVelocity((forwardSpeed + turnSpeed) * robot.Kspeed, percent);
        MotorLeft.setVelocity((forwardSpeed - turnSpeed) * robot.Kspeed, percent);
        MotorLeft.spin(reverse);
        MotorRight.spin(reverse);
    }
    if ((Controller.AxisA.position() > -5.0 and Controller.AxisA.position() < 5) and (Controller.AxisB.position() > -5.0 and Controller.AxisB.position() < 5))
    {
        // if (speedTurnAF > 50.0 || speedTurnAF < -50.0)
        // {
        //     MotorLeft.stop(hold);
        //     MotorRight.stop(hold);
        // }
        // else
        // {
        turnSpeed = 0;
        MotorLeft.setStopping(coast);
        MotorRight.setStopping(coast);
        MotorLeft.stop();

        MotorRight.stop();
        // }
    }
    // speedTurnAF = Controller.AxisB.position();
    // }
}

int control_drive()
{
    while (true)
    {
        if (robot.stop == false)
        {
            Drive();
        }
        wait(25, msec);
    }
}

// "when started" hat block
int start()
{
    MotorPin.setPosition(0.0, degrees);
    MotorBeam.setPosition(0.0, degrees);
    MotorPin.setVelocity(80.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    MotorBeam.setStopping(hold);
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    Retract_joint_pin();
    Retract_Pneumatic_Beam();
    Pneumatic_back.pumpOn();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Flying Bot CNX");
    Grab_then_up();
    wait(500, msec);
    Drop_down();
    Drop_down_beam();
    // TouchLED.setColor(red);
    Brain.Timer.reset();
    robot.NewDowm = false;
    return 0;
}

// "when Controller ButtonFDown pressed" hat block
void FDown_pressed()
{
    if (robot.isBusy == false)
    {
        Brain.Timer.reset();
        if (robot.beamState == BeamState::OFF)
        {
            // MotorBeam.setStopping(hold);
            MotorBeam.spin(forward);
            // robot.Maxturn = 1.0;
            Extend_Pneumatic_Beam();
            wait(500, msec);
            MotorBeam.spinFor(reverse, 120.0, degrees, false);
        }
        else
        {
            MotorBeam.setVelocity(100, percent);
            MotorBeam.spin(forward);
            wait(200, msec);
            MotorBeam.setStopping(brake);
            MotorBeam.stop();
            // robot.Maxturn = 0.55;
            Retract_Pneumatic_Beam();
        }
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("Fdown_Done: %.2f", robot.nowTime);
    }
}

// "when Controller ButtonFUp pressed" hat block
void FUp_pressed()
{
    if (robot.isBusy == false)
    {
        Brain.Timer.reset();
        if (robot.pinState == PinState::OFF)
        {
            MotorPin.resetPosition();
            Pneumatic_font.extend(cylinder2);
            Pneumatic_back.extend(cylinder1);
            robot.pinState = PinState::ON;
            wait(100, msec);
            MotorPin.resetPosition();
            if (robot.NewDowm == true)
            {
                MotorPin.setVelocity(100, percent);
                MotorPin.spinFor(forward, 40.0, degrees, false);
                // MotorPin.spin(forward, 100, pct);
                // wait(150, msec);
                // MotorPin.setStopping(hold);
                // MotorPin.stop();
            }
        }
        else if (robot.pinState == PinState::ON)
        {
            if ((controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2) && robot.Go == true)
            {
                robot.Go = false;
                robot.EDown = false;
                robot.Eup = false;
            }
            wait(100, msec);
            if (robot.NewDowm == true)
            {
                // MotorPin.setStopping(coast);
                MotorPin.stop(coast);
                robot.NewDowm = false;
            }
            if (Rdown_on == true)
            {
                Pneumatic_font.retract(cylinder2);
                Pneumatic_back.retract(cylinder1);
                wait(200, msec);
                RDown_pressed();
                Rdown_on = false;
            }
            else
            {
                Pneumatic_font.retract(cylinder2);
                Pneumatic_back.retract(cylinder1);
            }
            if (Endgame == true)
            {
                wait(500, msec);
                EDown_pressde();
                Endgame = false;
            }
            robot.pinState = PinState::OFF;
            wait(100, msec);
        }
    }
}

// "when Controller ButtonRUp pressed" hat block
void RUp_pressed()
{
    if (robot.isBusy == false)
    {
        robot.isBusy = true;
        Rup_Init();
        if (controllerState.Rup == ButtonStage::IDLE)
        {
            Grab_then_up();
            controllerState.Rup = ButtonStage::STEP1;
            robot.pinState = PinState::ON;
            controllerState.Rdown = ButtonStage::STEP1;
        }
        else if (controllerState.Rup == ButtonStage::STEP1)
        {
            Drop_down_Grab_Up();
            robot.pinState = PinState::ON;
            controllerState.Rup = ButtonStage::STEP2;
        }
        else if (controllerState.Rup == ButtonStage::STEP2)
        {
            Drop_down_Grab_Up();
            robot.pinState = PinState::ON;
        }
        robot.isBusy = false;
    }
}

// "when started" hat block
int Time()
{
    Brain.Timer.reset();
    while (true)
    {
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Pin: %2f ", MotorPin.position(degrees));
        if ((Brain.Timer.value() > 240.0))
        {
            Brain.programStop();
        }
        // if (robot.Maxturn == 100.0)
        // {
        //     TouchLED.setColor(green);
        // }
        wait(5, msec);
    }
    return 0;
}

// "when Controller ButtonRDown pressed" hat block
void RDown_pressed()
{
    if (robot.isBusy == false && (controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2 || robot.Eup == false))
    {
        robot.isBusy = true;
        Rdown_Init();
        Drop_down();
        robot.isBusy = false;
        robot.Go = false;
        controllerState.Rup = ButtonStage::IDLE;
        controllerState.Rdown = ButtonStage::IDLE;
    }
}

void EUp_pressed()
{
    if (robot.isBusy == false)
    {
        robot.stop = true;
        MotorLeft.setVelocity(40, pct);
        MotorRight.setVelocity(100, pct);
        MotorLeft.spin(forward);
        MotorRight.spin(forward);
        wait(1000, msec);
        robot.stop = false;
    }
}

void EDown_pressde()
{
    if (robot.isBusy == false)
    {
        Pin_On_the_sidelines();
    }
}

void ControllerDChanged()
{
    if ((controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2) && Controller.AxisD.position() >= 90.0)
    {
        //
        if (robot.isBusy == false)
        {
            robot.stop = true;
            robot.Go = true;
            MotorLeft.setVelocity(100, pct);
            MotorRight.setVelocity(100, pct);
            RUp_pressed();
        }
    }
    else if ((controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2) && Controller.AxisD.position() <= -90.0)
    {
        if (robot.isBusy == false)
        {
            robot.stop = true;
            robot.EDown = true;
            MotorLeft.setVelocity(100, pct);
            MotorRight.setVelocity(100, pct);
            RUp_pressed();
        }
    }
}

// "when Controller AxisC changed" hat block
// void ControllerCChanged()
// {
//     if (robot.isBusy == false && controllerState.Rup == ButtonStage::STEP1)
//     {
//         robot.stop = true;
//         wait(100, msec);
//         robot.stop = true;
//         standoff();
//         robot.stop = false;
//     }
// }
// void ControllerButtonL3_pressed()
// {
//     if (robot.isBusy == false && controllerState.Rup == ButtonStage::STEP1)
//     {
//         robot.Eup = false;
//         Pneumatic_font.retract(cylinder1);
//         Retract_joint_pin();
//         MotorPin.resetPosition();
//         MotorPin.setVelocity(70, percent);
//         // Robot NO2 = 130.0
//         // Robot NO3 = 113.0
//         MotorPin.spinFor(reverse, 110.0, degrees, true);
//         MotorPin.setVelocity(100, percent);
//         // blue_team
//         // Pneumatic_font.retract(cylinder2);
//         // red_team
//         Pneumatic_back.retract(cylinder1);
//         robot.Go = true;
//     }
//     robot.stop = false;
//     // Brain.Screen.clearLine(3, 1);
//     // robot.nowTime = Brain.Timer.value();
//     // Brain.Screen.setCursor(3, 1);
//     // Brain.Screen.print("L3_Done: %.2f", robot.nowTime);
// }

// "when Controller ButtonR3 pressed" hat block
void ControllerButtonR3_pressed()
{
    if (robot.isBusy == false && (controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2))
    {
        robot.isBusy = true;
        flip();
        robot.isBusy = false;
    }
}

void Extend_Pneumatic_Beam()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_font.extend(cylinder1);
    robot.beamState = BeamState::ON;
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // // Brain.Screen.print("Beam_E: %.2f", robot.nowTime);
}

void Retract_Pneumatic_Beam()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_font.retract(cylinder1);
    robot.beamState = BeamState::OFF;
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("Beam_R: %.2f", robot.nowTime);
    return;
}
//---
void Extend_joint_pin()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_back.retract(cylinder2);
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("gPin_E: %.2f", robot.nowTime);
}

void Retract_joint_pin()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_back.extend(cylinder2);
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("gPin_R: %.2f", robot.nowTime);
}

void Extend_2Pneumatic_Pin()
{
    Pneumatic_font.extend(cylinder2);
    Pneumatic_back.extend(cylinder1);
    // robot.pinState = PinState::ON;
}

void Retract_2Pneumatic_Pin()
{
    Pneumatic_font.retract(cylinder2);
    Pneumatic_back.retract(cylinder1);
    // robot.pinState = PinState::OFF;
}

//--
int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();
    start();
    // register event handlers
    Controller.ButtonLUp.pressed(LUp_pressed);
    Controller.ButtonLDown.pressed(LDown_pressed);
    Controller.ButtonFDown.pressed(FDown_pressed);
    Controller.ButtonEDown.pressed(EDown_pressde);
    Controller.ButtonFUp.pressed(FUp_pressed);
    Controller.ButtonRUp.pressed(RUp_pressed);
    Controller.ButtonRDown.pressed(RDown_pressed);
    Controller.ButtonEUp.pressed(EUp_pressed);
    Controller.AxisD.changed(ControllerDChanged);
    // Controller.AxisC.changed(ControllerCChanged);
    Controller.ButtonR3.pressed(ControllerButtonR3_pressed);
    // Controller.ButtonL3.pressed(ControllerButtonL3_pressed);
    thread ws3_controller(control_drive);
    thread ws2_time(Time);
}