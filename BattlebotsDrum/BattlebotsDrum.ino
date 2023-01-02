#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <WiFi.h>

Servo BDC1;
Servo BDC2;
Servo BLDC1;
float value;
float xvalue;
float yvalue;
float bdc1calc;
float bdc2calc;

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
  //Black Genuine Controller
  PS4.begin("30:C6:F7:29:75:08");
  
  BDC1.attach(4); // PWM signal output from pin 2.
  BDC2.attach(2); // PWM signal output from pin 4.
  BLDC1.attach(5); // PWM signal output from pin 5.
  Serial.println("Ready.");
  //BLDC1.write(10);
  //delay(5000);
}

void loop() {
  if (PS4.isConnected()) {

    /*
    // L2 Weapon Control
    //Serial.printf("L2 button at %d\n", PS4.L2Value());
    value = map(PS4.L2Value(), 0, 255, 30, 70);
    BLDC1.write(value); // Adjusts the ESC / servo position according to the value from the controller.
    //Serial.print("Value = ");
    //Serial.println(value);
    */

    // Right Stick Weapon Control
    value = PS4.RStickY();
    if(value < - 20) {
      value = map(value, -128, -20, 90, 30);
    } else if(value > 20) {
      value = map(value, 30, 128, 30, 60); 
    } else {
      value = 30;
    }
    BLDC1.write(value);
    
    /*
    //No channel mixing arcade drive with L1 R1 turning
    if(PS4.L1()){
      bdc1calc = 180;
      bdc2calc = 180;
    }else if(PS4.R1()){
      bdc1calc = 0;
      bdc2calc = 0;
    }else{
      
      xvalue = map(PS4.RStickX(), -128, 127, -90, 90);
      yvalue = map(PS4.LStickY(), -128, 127, -90, 90);

      if(xvalue>0){
        bdc2calc = yvalue;
        bdc1calc = (yvalue-(.5*abs(xvalue)));
      }else{
        bdc2calc = (yvalue-(.5*abs(xvalue)));
        bdc1calc = yvalue;
      }

      Serial.print("right stick x value = ");
      Serial.println(PS4.RStickX());
      Serial.print("xvalue = ");
      Serial.println(xvalue);
      Serial.print("Motor 1 value = ");
      Serial.println(bdc1calc);
      Serial.print("Motor 2 value = ");
      Serial.println(bdc2calc);
    
      bdc1calc = map(bdc1calc, -90, 90, 0, 180);
      bdc2calc = map(bdc2calc, -90, 90, 180, 0);
      
      //Serial.printf("Left Stick y at %d\n", PS4.LStickY());
      //Serial.printf("Right Stick x at %d\n", PS4.RStickX());
  
      if(bdc1calc>80 and bdc1calc<110){
        bdc1calc = 97;
      }
      if(bdc2calc>80 and bdc2calc<110){
        bdc2calc = 97;
      }
    }
    */

    /*
    //Tank drive
    bdc1calc = map(PS4.RStickY(), -128, 127, 50, 142);
    bdc2calc = map(PS4.LStickY(), -128, 127, 142, 50);

    Serial.print("Motor 1 value = ");
    Serial.println(bdc1calc);
    Serial.print("Motor 2 value = ");
    Serial.println(bdc2calc);

    if(bdc1calc>85 and bdc1calc<105){
      bdc1calc = 99;
    }
    if(bdc2calc>85 and bdc2calc<105){
      bdc2calc = 99;
    }
    */

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
    bdc2calc = yvalue+xvalue; 
    bdc1calc = yvalue-xvalue;

    //Now we map those values to the range of values that the motor controller can read, which you may need to test, but usually seems to be from around 0-180.
    bdc1calc = map(bdc1calc, -90, 90, 50, 148); 
    bdc2calc = map(bdc2calc, -90, 90, 148, 50);

    //We need to create deadzones for the sticks on the controller, since their resting point isn't always at 0. This may need to be adjusted for your controller.
//    if(bdc1calc>80 and bdc1calc<100){ 
//      bdc1calc = 100;
//    }
//    if(bdc2calc>80 and bdc2calc<100){
//      bdc2calc = 100;
//    }

    Serial.print(bdc1calc);
    Serial.print(bdc2calc);
    
    //Write the values to the motor controllers.
    BDC1.write(bdc1calc); 
    BDC2.write(bdc2calc);
    */

    
    //Channel Mixing
    yvalue = map(PS4.LStickX(), -128, 127, -90, 90);
    xvalue = map(PS4.LStickY(), -128, 127, -90, 90);
    
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

    bdc2calc = yvalue+xvalue; 
    bdc1calc = yvalue-xvalue;

    bdc2calc = map(bdc2calc, -90, 90, 50, 148);
    bdc1calc = map(bdc1calc, -90, 90, 148, 50);

    BDC1.write(bdc2calc);
    BDC2.write(bdc1calc);
    
  }
}
