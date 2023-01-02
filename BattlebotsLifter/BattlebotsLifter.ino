#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <WiFi.h>

Servo servo;
Servo BLDC1;
Servo BLDC2;
float up;
float down;
float xvalue;
float yvalue;
float BLDC1calc;
float BLDC2calc;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Replace the "1a:2b:3c:01:01:01" with the MAC address
  // the controller wants to pair to
  // Note: MAC address must be unicast

  //Blue Genuine Controller
  //PS4.begin("78:21:84:7f:ee:40");
  //Black Knockoff Controller
  //PS4.begin("e8:d8:19:51:01:b2");
  //White Knockoff Controller
  PS4.begin("e8:d8:19:51:01:b3");
  //Black Genuine Controller
  //PS4.begin("30:C6:F7:29:75:08");

  servo.attach(2); // PWM signal output from pin 2.
  BLDC1.attach(4); // PWM signal output from pin 4.
  BLDC2.attach(5); // PWM signal output from pin 5.
  Serial.println("Ready.");
  //BLDC2.write(10);
  //delay(5000);
}

void loop() {
  if (PS4.isConnected()) {

    /*
      // L2 Weapon Control
      //Serial.printf("L2 button at %d\n", PS4.L2Value());
      value = map(PS4.L2Value(), 0, 255, 30, 70);
      BLDC2.write(value); // Adjusts the ESC / servo position according to the value from the controller.
      //Serial.print("Value = ");
      //Serial.println(value);
    */

    
    up = PS4.R2Value();
    down = PS4.L2Value();

    up = map(up, 0, 255, 65, 180);
    down = map(down, 0, 255, 65, 0);

    if (PS4.R2Value() > 2 and PS4.L2Value() < 2) {
      servo.write(up);
    }
    if (PS4.L2Value() > 2 and PS4.R2Value() < 2) {
      servo.write(down);
    }else{
      //servo.write(65);
    }

    Serial.print("down = ");
    Serial.println(down);
    Serial.print("up = ");
    Serial.println(up);
    

    /*
      //No channel mixing arcade drive with L1 R1 turning
      if(PS4.L1()){
      servocalc = 180;
      BLDC1calc = 180;
      }else if(PS4.R1()){
      servocalc = 0;
      BLDC1calc = 0;
      }else{

      xvalue = map(PS4.RStickX(), -128, 127, -90, 90);
      yvalue = map(PS4.LStickY(), -128, 127, -90, 90);

      if(xvalue>0){
        BLDC1calc = yvalue;
        servocalc = (yvalue-(.5*abs(xvalue)));
      }else{
        BLDC1calc = (yvalue-(.5*abs(xvalue)));
        servocalc = yvalue;
      }

      Serial.print("right stick x value = ");
      Serial.println(PS4.RStickX());
      Serial.print("xvalue = ");
      Serial.println(xvalue);
      Serial.print("Motor 1 value = ");
      Serial.println(servocalc);
      Serial.print("Motor 2 value = ");
      Serial.println(BLDC1calc);

      servocalc = map(servocalc, -90, 90, 0, 180);
      BLDC1calc = map(BLDC1calc, -90, 90, 180, 0);

      //Serial.printf("Left Stick y at %d\n", PS4.LStickY());
      //Serial.printf("Right Stick x at %d\n", PS4.RStickX());

      if(servocalc>80 and servocalc<110){
        servocalc = 97;
      }
      if(BLDC1calc>80 and BLDC1calc<110){
        BLDC1calc = 97;
      }
      }
    */

  
    //Tank drive
    if(PS4.RStickY()>30){
      BLDC1calc = map(PS4.RStickY(), 30, 127, 96, 101);
    }
    else if(PS4.RStickY()<-30){
      BLDC1calc = map(PS4.RStickY(), -127, -30, 85, 90);
    }
    else if(PS4.LStickY()>30){
      BLDC2calc = map(PS4.LStickY(), 30, 127, 96, 103);
    }
    else if(PS4.LStickY()<-30){
      BLDC2calc = map(PS4.LStickY(), -127, -30, 83, 90);
    }else{
      BLDC1calc = 93;
      BLDC2calc = 93;
    }
    
    Serial.print("Motor 1 value = ");
    Serial.println(BLDC1calc);
    Serial.print("Motor 2 value = ");
    Serial.println(BLDC2calc);
    
    BLDC1.write(BLDC1calc);
    BLDC2.write(BLDC2calc);

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
      BLDC1calc = yvalue+xvalue;
      servocalc = yvalue-xvalue;

      //Now we map those values to the range of values that the motor controller can read, which you may need to test, but usually seems to be from around 0-180.
      servocalc = map(servocalc, -90, 90, 50, 148);
      BLDC1calc = map(BLDC1calc, -90, 90, 148, 50);

      //We need to create deadzones for the sticks on the controller, since their resting point isn't always at 0. This may need to be adjusted for your controller.
      //    if(servocalc>80 and servocalc<100){
      //      servocalc = 100;
      //    }
      //    if(BLDC1calc>80 and BLDC1calc<100){
      //      BLDC1calc = 100;
      //    }

      Serial.print(servocalc);
      Serial.print(BLDC1calc);

      //Write the values to the motor controllers.
      servo.write(servocalc);
      BLDC1.write(BLDC1calc);
    */

    /*
      //Channel Mixing
      xvalue = map(PS4.LStickX(), -128, 127, -90, 90);
      yvalue = map(PS4.LStickY(), 0, 127, 0, 90);

      if(xvalue > -20 and xvalue < 20){
      xvalue = 0;
      }
      if(yvalue > -20 and yvalue < 20){
      yvalue = 0;
      }

      Serial.print("Motor x value = ");
      Serial.println(xvalue);
      Serial.print("Motor y value = ");
      Serial.println(yvalue);

      BLDC2calc = yvalue+xvalue;
      BLDC1calc = yvalue-xvalue;

      if(BLDC2calc < 0){
      BLDC2calc = 0;
      }
      if(BLDC1calc < 0){
      BLDC1calc = 0;
      }

      BLDC2calc = map(BLDC2calc, 0, 90, 25, 35);
      BLDC1calc = map(BLDC1calc, 0, 90, 25, 35);

      BLDC1.write(BLDC1calc);
      BLDC2.write(BLDC2calc);
    */

    /*
      Try this test sketch with the Servo library to see how your
      servo responds to different settings, type a position
      (0 to 180) or if you type a number greater than 200 it will be
      interpreted as microseconds(544 to 2400), in the top of serial
      monitor and hit [ENTER], start at 90 (or 1472) and work your
      way toward zero (544) 5 degrees (or 50 micros) at a time, then
      toward 180 (2400).
    */

    /*
    //testing servo
    // if there's any serial available, read it:
    while (Serial.available() > 0) {

      // look for the next valid integer in the incoming serial stream:
      int pos = Serial.parseInt();
      pos = constrain(pos, 0, 2400);
      servo.write(pos);
      prntIt();
    }
    */

  }
}

void prntIt()
    {
      Serial.print("  degrees = ");
      Serial.print(servo.read());
      Serial.print("\t");
      Serial.print("microseconds =  ");
      Serial.println(servo.readMicroseconds());
    }
