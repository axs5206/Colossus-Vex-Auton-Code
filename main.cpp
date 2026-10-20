#include "main.h"

// Global Motor Group Definitions
pros::MotorGroup left_mg({-16, -17, -18});
pros::MotorGroup right_mg({13, 14, 15}); 
pros::MotorGroup tracking_mg({-19, 20});

void driveFor(int speed, int milliseconds) {
    const int limitedSpeed = speed > 127 ? 127 : (speed < -127 ? -127 : speed);
    if (milliseconds <= 0) {
        left_mg.move(0);
        right_mg.move(0);
        //tracking_mg.move(0);
        return;
    }

    left_mg.move(limitedSpeed);
    right_mg.move(limitedSpeed);
    pros::delay(milliseconds);
    left_mg.move(0);
    right_mg.move(0);
}

void turnFor(int speed, int milliseconds) {
    const int limitedSpeed = speed > 127 ? 127 : (speed < -127 ? -127 : speed);
    if (milliseconds <= 0) {
        left_mg.move(0);
        right_mg.move(0);
        return;
    }

    left_mg.move(limitedSpeed);
    right_mg.move(-limitedSpeed);
    pros::delay(milliseconds);
    left_mg.move(0);
    right_mg.move(0);
}

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "1-Joystick Test Ready");
}

void disabled() {}
void competition_initialize() {}
void autonomous() {
    driveFor(60, 1000);
    turnFor(45, 500);
    driveFor(73, 500);
    tracking_mg.move(127);
    pros::delay(1000);
    tracking_mg.move(0);
    turnFor(-50, 500);
    driveFor(80, 1000);
}

/**
 * Runs the operator control code with a single joystick setup.
 */
void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            autonomous();
            continue;
        }

        // Single Left Joystick Arcade Control
        // ANALOG_LEFT_Y: Forward / Backward movement
        // ANALOG_LEFT_X: Left / Right turning movement
        int power = master.get_analog(ANALOG_LEFT_Y);    
        int turn = master.get_analog(ANALOG_LEFT_X);  

        if (power > -5 && power < 5) power = 0;
        if (turn > -5 && turn < 5) turn = 0;

        int leftSpeed = power + turn;
        int rightSpeed = power - turn;
        int largestSpeed = leftSpeed > rightSpeed ? leftSpeed : rightSpeed;
        if (-leftSpeed > largestSpeed) largestSpeed = -leftSpeed;
        if (-rightSpeed > largestSpeed) largestSpeed = -rightSpeed;
        if (largestSpeed > 127) {
            leftSpeed = leftSpeed * 127 / largestSpeed;
            rightSpeed = rightSpeed * 127 / largestSpeed;
        }

        left_mg.move(leftSpeed);
        right_mg.move(rightSpeed);

        pros::delay(20); // Run loop every 20ms
    }
}

