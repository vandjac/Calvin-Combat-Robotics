#include <PS4Controller.h> //PS4 controller library that will allow us to communicate with the controller and read its outputs
#include <ESP32Servo.h> //Library that allows us to control servo motors, brushed motors, and brushless motors using PWM signals 
//#include <WiFi.h>

// Hello World

//For erasing flash: python -m esptool --chip esp32 erase_flash

//Declare objects for each motor. BDC stands for brushed DC and BLDC stands for brushless DC
Servo BDC1;
Servo BDC2;
Servo BLDC1;
//Initialize our variables (variables will be explained as they are used)
float value;
float xvalue;
float yvalue;
float driveMotor1;
float driveMotor2;
float forwardSpeed = 128;
float reverseSpeed = 70;
float forwardSpeedTemp = forwardSpeed;
float reverseSpeedTemp = reverseSpeed;
float xvalueDelay = 0;
float yvalueDelay = 0;
float rumble = 0;
float leftRumble = 0;
float rightRumble = 0;
bool reversedTurn = false;

//In the setup function is code that only runs once at the start of the program (when your robot is turned on)
void setup() {
  //Print the esp-32's MAC address (you probably won't need this)
  Serial.begin(115200);
  //Serial.println();
  //Serial.print("ESP Board MAC Address:  ");
  //Serial.println(WiFi.macAddress());

  //Uncomment the code for the controller you want to pair to below. If you're using your own controller that isn't listed here, write another line and put its MAC address within PS4.begin();

  //Blue Genuine Controller
  //PS4.begin("78:21:84:7f:ee:40");
  //Black Knockoff Controller
  PS4.begin("e8:d8:19:51:01:b2");
  //Black Genuine Controller
  //PS4.begin("30:C6:F7:29:75:08");
  //White Knockoff Controller
  //PS4.begin("e8:d8:19:51:01:b3");
  //Controller 6
  //PS4.begin("e8:d8:19:51:01:b6");
  //Controller 5
  //PS4.begin("e8:d8:19:51:01:b5");
  //Controller 4
  //PS4.begin("e8:d8:19:51:01:b4");
  //Kyle's controller
  //PS4.begin("2c:dc:d7:03:04:c0");

  //Assign motor signal wires to their pins on the esp-32
  BDC1.attach(2); // PWM signal output from pin 2.
  BDC2.attach(4); // PWM signal output from pin 4.
  BLDC1.attach(5); // PWM signal output from pin 5.

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


    // L2 Weapon Control
    if(PS4.R2Value()>10){
      value = map(PS4.R2Value(), 0, 255, 30, 90);
      rightRumble = PS4.R2Value();
    }
    if(PS4.L2Value()>10){
      value = map(PS4.L2Value(), 0, 255, 30, 60);
      leftRumble = PS4.L2Value();
    }
    if(PS4.R2Value()<10 and PS4.L2Value()<10){
      leftRumble = rightRumble = 0;
      value = 30;
    }
    BLDC1.write(value);
    PS4.setRumble(leftRumble, rightRumble);
    PS4.sendToController();

    /*
      // Right Stick Weapon Control
      value = PS4.RStickY();
      if (value < - 20) {
      value = map(value, -128, -20, 90, 30);
      rumble = map(value, -128, -30, 255, 0);
      } else if (value > 20) {
      value = map(value, 30, 128, 30, 60);
      rumble = map(value, 40, 128, 150, 0);
      } else {
      value = 30;
      rumble = 0;
      }
      BLDC1.write(value);

      PS4.setRumble(rumble, rumble);
      PS4.sendToController();
    */

    
      //Tank drive
      driveMotor1 = map(PS4.RStickY(), -128, 127, 50, 142);
      driveMotor2 = map(PS4.LStickY(), -128, 127, 50, 142);

      Serial.print("Motor 1 value = ");
      Serial.println(driveMotor1);
      Serial.print("Motor 2 value = ");
      Serial.println(driveMotor2);

      if(driveMotor1>85 and driveMotor1<105){
      driveMotor1 = 99;
      }
      if(driveMotor2>85 and driveMotor2<105){
      driveMotor2 = 99;
      }

    /*
      //Normal arcade drive
      //Take the x value of the right stick (which is anywhere from -128 to 128), and convert it to a value from -90 to 90. This is so we have a nice range of 180 "degrees".
      xvalue = map(PS4.RStickX(), -128, 127, -90, 90);
      yvalue = map(PS4.LStickY(), -128, 127, -90, 90);

      // Deadzone calculations
      if(xvalue < 10 and xvalue > -10) {
      xvalue = 0;
      }
      if(yvalue <10 and yvalue > -10) {
      yvalue = 0;
      }

      //The y value controls the speed, and the x value controls the direction. In order to change direction, the speed of one of the motors needs to be greater than the other. Therefore, we add the direction value to one motor and subtract it from the other.
      driveMotor2 = yvalue+xvalue;
      driveMotor1 = yvalue-xvalue;

      //Now we map those values to the range of values that the motor controller can read, which you may need to test, but usually seems to be from around 0-180.
      driveMotor1 = map(driveMotor1, -90, 90, 50, 148);
      driveMotor2 = map(driveMotor2, -90, 90, 148, 50);

      //We need to create deadzones for the sticks on the controller, since their resting point isn't always at 0. This may need to be adjusted for your controller.
      //    if(driveMotor1>80 and driveMotor1<100){
      //      driveMotor1 = 100;
      //    }
      //    if(driveMotor2>80 and driveMotor2<100){
      //      driveMotor2 = 100;
      //    }

      Serial.print(driveMotor1);
      Serial.print(driveMotor2);

      //Write the values to the motor controllers.
      BDC1.write(driveMotor1);
      BDC2.write(driveMotor2);
    */

    /*
      //Channel Mixing (use only the left stick to drive)

      //Take values from joysticks (from -128 to 127) and map them to a more usable range (-90 to 90)
      xvalue = map(PS4.LStickY(), -128, 127, -90, 90);
      yvalue = map(PS4.LStickX(), -128, 127, 90, -90);

      //All of these if statements are used to limit the maximum acceleration of the drive motors.
      //You want this because if you go from moving forward at max speed to moving backward at max speed instantly (moving joystick up to down instantly) you will wear out the gears in the gearbox of your motor (not good!)

      //If the joystick x value is low, set the xvalue to zero (create a deadzone)
      if (xvalue > -20 and xvalue < 20) {
      xvalue = 0;
      }
      //xvalueDelay and yvalueDelay are delayed versions of the xvalue and yvalue. They will try to reach xvalue and yvalue, but will be slightly behind in order to create a more smooth response from the motor
      if (xvalueDelay < xvalue) {
      xvalueDelay += 1;
      delay(1);
      }
      if (xvalueDelay > xvalue) {
      xvalueDelay -= 1;
      delay(1);
      }
      if (yvalue > -20 and yvalue < 20) {
      yvalue = 0;
      }
      if (yvalueDelay < yvalue) {
      yvalueDelay += 5;
      delay(1);
      }
      if (yvalueDelay > yvalue) {
      yvalueDelay -= 5;
      delay(1);
      }

      //Print out motor values (used for debugging, you probably wont need this)
      Serial.print("Motor x value = ");
      Serial.println(xvalueDelay);
      Serial.print("Motor y value = ");
      Serial.println(yvalueDelay);

      //Calculate the speed and direction of the motors based on where the joysticks are pointing.
      //If y and x are positive (joystick is pointed up and to the right) then the motor on the left will need to move forward faster than the motor on the right.
      //This is a bit confusing but if you think through each scenario like this, you should see how it works.
      driveMotor2 = yvalueDelay + xvalueDelay;
      driveMotor1 = yvalueDelay - xvalueDelay;

      //Map the motor values from -90 to 90 to 50 to 148 (numbers that are meaningful to the motor controller).
      //Brushed and brushless motors generally have a range from 0-180, where 0 is fully backwards, 90 is neatral, and 180 is fully forwards.
      //In practice, that isn't always perfectly true. For instance, you way need to play around with the middle value (it might be 85 instead of 90 for example).
      driveMotor2 = map(driveMotor2, -90, 90, forwardSpeed, reverseSpeed);
      driveMotor1 = map(driveMotor1, -90, 90, reverseSpeed, forwardSpeed);

      //Write the values to the motors
      if (reversedTurn == false) {
      BDC1.write(driveMotor1);
      BDC2.write(driveMotor2);
      } else {
      BDC1.write(driveMotor2);
      BDC2.write(driveMotor1);
      }
    */
    /*
    //Normal Arcade Drive
    //Take values from joysticks (from -128 to 127) and map them to a more usable range (-90 to 90)
    xvalue = map(PS4.LStickY(), -128, 127, -90, 90);
    yvalue = map(PS4.RStickX(), -128, 127, 90, -90);

    //All of these if statements are used to limit the maximum acceleration of the drive motors.
    //You want this because if you go from moving forward at max speed to moving backward at max speed instantly (moving joystick up to down instantly) you will wear out the gears in the gearbox of your motor (not good!)

    //If the joystick x value is low, set the xvalue to zero (create a deadzone)
    if (xvalue > -10 and xvalue < 10) {
      xvalue = 0;
    }
    //xvalueDelay and yvalueDelay are delayed versions of the xvalue and yvalue. They will try to reach xvalue and yvalue, but will be slightly behind in order to create a more smooth response from the motor
    if (xvalueDelay < xvalue) {
      xvalueDelay += 1;
      delay(1);
    }
    if (xvalueDelay > xvalue) {
      xvalueDelay -= 1;
      delay(1);
    }
    if (yvalue > -20 and yvalue < 20) {
      yvalue = 0;
    }
    if (yvalueDelay < yvalue) {
      yvalueDelay += 5;
      delay(1);
    }
    if (yvalueDelay > yvalue) {
      yvalueDelay -= 5;
      delay(1);
    }

    //Print out motor values (used for debugging, you probably wont need this)
    Serial.print("Motor x value = ");
    Serial.println(xvalueDelay);
    Serial.print("Motor y value = ");
    Serial.println(yvalueDelay);

    //Calculate the speed and direction of the motors based on where the joysticks are pointing.
    //If y and x are positive (joystick is pointed up and to the right) then the motor on the left will need to move forward faster than the motor on the right.
    //This is a bit confusing but if you think through each scenario like this, you should see how it works.
    driveMotor2 = yvalueDelay + xvalueDelay;
    driveMotor1 = yvalueDelay - xvalueDelay;

    //Map the motor values from -90 to 90 to 50 to 148 (numbers that are meaningful to the motor controller).
    //Brushed and brushless motors generally have a range from 0-180, where 0 is fully backwards, 90 is neatral, and 180 is fully forwards.
    //In practice, that isn't always perfectly true. For instance, you way need to play around with the middle value (it might be 85 instead of 90 for example).
    driveMotor2 = map(driveMotor2, -90, 90, forwardSpeed, reverseSpeed);
    driveMotor1 = map(driveMotor1, -90, 90, reverseSpeed, forwardSpeed);
    */
    //Write the values to the motors
    if (reversedTurn == false) {
      BDC1.write(driveMotor1);
      BDC2.write(driveMotor2);
    } else {
      BDC1.write(driveMotor2);
      BDC2.write(driveMotor1);
    }
  }
}
void onEvent() {
  //Flip forward/reverse direction of robot with x button (if you wired it wrong, or you want to drive your robot backwards, or you're driving your robot upside down)
  if (PS4.event.button_down.cross) {
    if (forwardSpeed == forwardSpeedTemp) {
      forwardSpeed = reverseSpeedTemp;
      reverseSpeed = forwardSpeedTemp;
    } else {
      forwardSpeed = forwardSpeedTemp;
      reverseSpeed = reverseSpeedTemp;
    }
  }
  //Flip turning direction of robot with square button (if you wired it wrong, or you want to drive your robot backwards, or you're driving your robot upside down)
  if (PS4.event.button_down.square) {
    reversedTurn = !reversedTurn;
  }
}
