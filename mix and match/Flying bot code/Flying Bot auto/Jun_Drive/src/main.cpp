/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Robot:        No.3_Teamwork                                             */
/*    Created:      11/17/2025, 12:59:47 PM                                   */
/*    Last edited:  11/20/2025, 9:10:00 AM                                    */
/*    patch:        11/26/2025, 11:26:00 AM                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
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
double Kspeed = 1.0, Lup = 0.0, MotorLeft_vel = 0.0, MotorRight_vel = 0.0;
bool front_back, stop = false;

void EUp_pressed()
{
    MotorLeft_vel += 1;
    printf("MotorLeft_vel: %.2f\n", MotorLeft_vel);
}

void EDown_pressed()
{
    MotorLeft_vel -= 1;
    printf("MotorLeft_vel: %.2f\n", MotorLeft_vel);
}

void FUp_pressed()
{
    MotorRight_vel += 1;
    printf("MotorRight_vel: %.2f\n", MotorRight_vel);
}

void FDown_pressed()
{
    MotorRight_vel -= 1;
    printf("MotorRight_vel: %.2f\n", MotorRight_vel);
}

void RUp_pressed(){
    stop = true;
    MotorLeft.setVelocity(MotorLeft_vel, percent);
    MotorRight.setVelocity(MotorRight_vel, percent);
    MotorLeft.spinFor(forward, 1000.0, degrees, false);
    MotorRight.spinFor(forward, 1000.0, degrees, true);
    stop = false;
}

void RDown_pressed(){
    stop = true;
    MotorLeft.setVelocity(MotorLeft_vel, percent);
    MotorRight.setVelocity(MotorRight_vel, percent);
    MotorLeft.spinFor(reverse, 1000.0, degrees, false);
    MotorRight.spinFor(reverse, 1000.0, degrees, true);
    stop = false;
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

int seting()
{
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

int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();

    // register event handlers
    Controller.ButtonFDown.pressed(FDown_pressed);
    Controller.ButtonEDown.pressed(EDown_pressed);
    Controller.ButtonFUp.pressed(FUp_pressed);
    Controller.ButtonRUp.pressed(RUp_pressed);
    Controller.ButtonRDown.pressed(RDown_pressed);
    Controller.ButtonEUp.pressed(EUp_pressed);

    thread ws3_controller(seting);
    // thread ws4_autoplace(autoBeam);
    // vex::task ws3(whenStarted4);
}