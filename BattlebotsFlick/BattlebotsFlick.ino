#include <PS4Controller.h> //PS4 controller library that will allow us to communicate with the controller and read its outputs
#include <ESP32Servo.h> //Library that allows us to control servo motors, brushed motors, and brushless motors using PWM signals 
//#include <WiFi.h>

//Declare objects for each motor. BDC stands for brushed DC and BLDC stands for brushless DC
Servo BDC1;
Servo BDC2;
Servo BDC3;
Servo BDC4;
//Initialize our variables (variables will be explained as they are used)
float value;
float xvalue;
float yvalue;
float driveMotor1;
float driveMotor2;
float forwardSpeed = 148;
float reverseSpeed = 50;
float xvalueDelay = 0;
float yvalueDelay = 0;
bool reversedTurn = false;

//In the setup function is code that only runs once at the start of the program (when your robot is turned on)
void setup() {
  //Print the esp-32's MAC address (you probably won't need this)
  Serial.begin(115200);
  //Serial.print("ESP Board MAC Address:  ");
  //Serial.println(WiFi.macAddress());

  //Uncomment the code for the controller you want to pair to below. If you're using your own controller that isn't listed here, write another line and put its MAC address within PS4.begin();

  //Blue Genuine Controller
  PS4.begin("78:21:84:7f:ee:40");
  //Black Knockoff Controller
  //PS4.begin("e8:d8:19:51:01:b2");
  //Black Genuine Controller
  //PS4.begin("30:C6:F7:29:75:08");
  //White Knockoff Controller
  //PS4.begin("e8:d8:19:51:01:b3");
  //Controller #5
  //PS4.begin("e8:d8:19:51:01:b5");

  //Assign motor signal wires to their pins on the esp-32
  BDC1.attach(2); // PWM signal output from pin 2.
  BDC2.attach(4); // PWM signal output from pin 4.
  BDC3.attach(5); // PWM signal output from pin 5.
  BDC4.attach(18); // PWM signal output from pin 18.

  PS4.attach(onEvent);

  PS4.setLed(185, 9, 19); //Set the controller's light bar color! Uses r,g,b
  PS4.sendToController(); //Send data to controller (used for setting light bar color and setting rumble)

  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    //vTaskDelete(NULL);
    //Within here there is a lot of code that is commented out. Don't be intimidated.
    //Most of it provides different ways of doing things (like using L2 for your weapon instead of the right stick).
    //If you want to use them, uncomment them and comment out the other version.
    //Disclaimer: if you use any of the commented out code, I don't guarantee they will work. I worked on them a long time ago and haven't tried them recently.

    if(PS4.R1()){
      value = 160;
    }else if(PS4.L1()){
      value = 30;
    }else{
      value = 95;
    }
    BDC1.write(value);

    //Normal Arcade Drive
    //Take values from joysticks (from -128 to 127) and map them to a more usable range (-90 to 90)
    xvalue = map(PS4.LStickY(), -128, 127, -90, 90);
    yvalue = map(PS4.RStickX(), -128, 127, 90, -90);

    //All of these if statements are used to limit the maximum acceleration of the drive motors.
    //You want this because if you go from moving forward at max speed to moving backward at max speed instantly (moving joystick up to down instantly) you will wear out the gears in the gearbox of your motor (not good!)

    //If the joystick x value is low, set the xvalue to zero (create a deadzone)
    if (xvalue > -20 and xvalue < 20) {
      xvalue = 0;
    }

    if (yvalue > -20 and yvalue < 20) {
      yvalue = 0;
    }

    //Print out motor values (used for debugging, you probably wont need this)
    Serial.print("Motor x value = ");
    Serial.println(xvalue);
    Serial.print("Motor y value = ");
    Serial.println(yvalue);

    //Calculate the speed and direction of the motors based on where the joysticks are pointing.
    //If y and x are positive (joystick is pointed up and to the right) then the motor on the left will need to move forward faster than the motor on the right.
    //This is a bit confusing but if you think through each scenario like this, you should see how it works.
    driveMotor2 = yvalue + xvalue;
    driveMotor1 = yvalue - xvalue;

    //Map the motor values from -90 to 90 to 50 to 148 (numbers that are meaningful to the motor controller).
    //Brushed and brushless motors generally have a range from 0-180, where 0 is fully backwards, 90 is neutral, and 180 is fully forwards.
    //In practice, that isn't always perfectly true. For instance, you way need to play around with the middle value (it might be 85 instead of 90 for example).
    driveMotor2 = map(driveMotor2, -90, 90, forwardSpeed, reverseSpeed);
    driveMotor1 = map(driveMotor1, -90, 90, reverseSpeed, forwardSpeed);

    //Write the values to the motors
    if (reversedTurn == false) {
      BDC3.write(driveMotor1);
      BDC4.write(driveMotor2);
    } else {
      BDC3.write(driveMotor2);
      BDC4.write(driveMotor1);
    }
  }
}
void onEvent() {
  //Flip forward/reverse direction of robot with x button (if you wired it wrong, or you want to drive your robot backwards, or you're driving your robot upside down)
  if (PS4.event.button_down.cross) {
    if (forwardSpeed == 148) {
      forwardSpeed = 50;
      reverseSpeed = 148;
    } else {
      forwardSpeed = 148;
      reverseSpeed = 50;
    }
  }
  //Flip turning direction of robot with square button (if you wired it wrong, or you want to drive your robot backwards, or you're driving your robot upside down)
  if (PS4.event.button_down.square) {
    reversedTurn = !reversedTurn;
  }
}
