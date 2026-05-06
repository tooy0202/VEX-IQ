#include "vex.h"
using namespace vex;

brain Brain;
inertial BrainInertial = inertial();
touchled TouchLED12 = touchled(PORT12);

bool Cal = true;

void vexcodeInit() {
  // nothing special for now
}

// ---------- PARALLEL: print heading ----------
int printHeadingTask() {
  while (true) {
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Heading: %.2f", BrainInertial.heading(degrees));
    wait(50, msec);
  }
  return 0;
}

// ---------- Wait for button + calibrate ----------
int whenStarted1() {
  TouchLED12.setColor(red);

  // 1. Wait until TouchLED is pressed
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("Touch to calibrate");

  while (!TouchLED12.pressing()) {
    wait(10, msec);
  }

  // 2. Optional: wait for release so it only triggers once
  while (TouchLED12.pressing()) {
    wait(10, msec);
  }

  // 3. Calibrate
  TouchLED12.setColor(orange);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("Calibrating...");

  BrainInertial.calibrate();
  while (BrainInertial.isCalibrating()) {
    wait(50, msec);
  }

  TouchLED12.setColor(blue);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("Done!");

  Cal = false;
  return 0;
}

int main() {
  vexcodeInit();

  // Wait for button & calibrate (BLOCKING)
  whenStarted1();

  // Start printing heading in PARALLEL
  task headingTask(printHeadingTask);   // <<< function name, no ()

  // Main robot loop
  while (true) {
    // put your driving / turning / whatever here
    wait(20, msec);
  }
}