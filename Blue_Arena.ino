//Code For Blue Mode
#include<avr/wdt.h>
#include <EEPROM.h>
#include <PS4USB.h>
#include <SPI.h>
#include<Wire.h>
#include<string.h>
#include<math.h>

//  ps4 variable
USB Usb;
PS4USB PS4(&Usb);
//


//       motor configuration:
//---------------------------------------------
//     | motor2(fld,fla)   |   motor1(frd,fra) |
//---------------------------------------------
//     | motor3(bld,bla)   |   motor4(brd,bra) |
//---------------------------------------------

//Ampflow motor pins and variables
byte lmp = 7, lmd = 29 , rmp = 5 , rmd = 25; //Left is Ampflow and right is planetery
int pwml = 0 , pwmr = 0, LIMIT_PWM_LEFT = 150, LIMIT_PWM_RIGHT = 180;
//

//
float last_imu_value = 0.0,present_imu_value = 0.0;
//

//Linear actuator pins
//byte lna = 9, lnd = 35;
byte lna = 6, lnd = 27;
//

//Throwing Mechanism Pins
//byte thrd = 27, thrp = 6;
byte thrd = 35, thrp = 9;
//

//kinematics matrix variable
float a11 = -0.35, a12 = 0.35, a13 = 0.25;
float a21 = -0.35, a22 = -0.35, a23 = 0.25;
float a31 = 0.35, a32 = -0.35, a33 = 0.25;
float a41 = 0.35, a42 = 0.35, a43 = 0.25;
//


//imu variable
#define BUFF_SIZE 40
char buff[BUFF_SIZE];
float comp_ref_yaw, ref_yaw;
String yaw_string, pitch_string, roll_string;
//

float w = 0, x = 0, y = 0; //inverse kinemaatics variables

//Node count and change
int node = 0;//Maximum 8 Nodes
int Left_speed[] = {0, 53, 56, 56, 56, 53, 38, 78}; //Ampflow Motor Speed
int Right_speed[] = {0, 70, 56, 69, 56, 70, 48, 66}; //Controlling Motor Speed
//

//Mode Change Variables
bool throwing = false;

void send_data(char ch , int x)
{
  String data = String(x);
  String Space = "0";
  String imu_data = String(abs((int)present_imu_value));
  String sending = "";
  sending += ch;
  sending += data;
  if(ch=='y' || ch=='n')
  {
    if( (int)present_imu_value > 0 ) Space = "1";
    sending += Space;
    sending += imu_data;
  }
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(sending.c_str());
  Wire.endTransmission();
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  motorSetup();
  ps4_setup();
  imusetup();
  //motion_check();
  wdt_enable(WDTO_1S);  
  EEPROM.write(0,node);
//  EEPROM.write(1,ref_yaw);
}

void loop()
{
  wdt_reset();
  node = EEPROM.read(0);
//  ref_yaw = EEPROM.read(1);
  Usb.Task();
  if (PS4.connected())
  {
    PS4.setLed(Blue);
    //Throwing Mode and headding mode interchange
    if (PS4.getButtonClick(OPTIONS))
    {
      throwing = !throwing;
    }
    //Linear Actuator control
    if(PS4.getButtonPress(LEFT))
    {
      digitalWrite(lna,HIGH);
      digitalWrite(lnd,HIGH); 
    }
    else if(PS4.getButtonPress(RIGHT))
    {
      digitalWrite(lna,HIGH);
      digitalWrite(lnd,LOW); 
    }
    else
    {
      digitalWrite(lna,LOW);
      digitalWrite(lnd,LOW); 
    }
    //
    //Node Count change and Speed Setting
    if (PS4.getButtonClick(CIRCLE))
    {
      if (node < 8)
        node++;
    //  send_data('n', node);
      pwml = Left_speed[node];
      pwmr = Right_speed[node];
     // send_data('l', pwml);
     // send_data('r', pwmr);
      EEPROM.write(0,node);
    }
    if (PS4.getButtonClick(SQUARE))
    {
      if (node > 0)
        node--;
     // send_data('n', node);
      pwml = Left_speed[node];
      pwmr = Right_speed[node];
      //send_data('l', pwml);
     // send_data('r', pwmr);
      EEPROM.write(0,node);
    }
    //

    //Initiate Motor Speed
    if (PS4.getButtonClick(PS))
    {
      pwml = Left_speed[node];
      pwmr = Right_speed[node];
      send_data('l', pwml);
      send_data('r', pwmr);
    }
    //
    //AmpFlow Motor Speed Control
    if (PS4.getButtonClick(UP))
    {
      if (pwml < LIMIT_PWM_LEFT)
        pwml++;//, send_data('l', pwml);
    }
    if (PS4.getButtonClick(DOWN))
    {
      if (pwml > 0)
        pwml--;//, send_data('l', pwml);
    }

    //Controlling throwing Motor's Speed 
    if (PS4.getButtonClick(TRIANGLE))
    {
      if (pwmr < LIMIT_PWM_RIGHT)
        pwmr++;//, send_data('r', pwmr);
    }
    if (PS4.getButtonClick(CROSS))
    {
      if (pwmr > 0)
        pwmr--;//, send_data('r', pwmr);
    }
    analogWrite(lmp, pwml);
    digitalWrite(lmd, LOW);
    analogWrite(rmp, pwmr);
    digitalWrite(rmd, LOW);

    //Click R1 to throw
    // Pneumatic actuator
    if (PS4.getButtonPress(R1))
    {
      digitalWrite(thrp, HIGH);
      digitalWrite(thrd, HIGH);
      //delay(100);
    }
    else
    {
      digitalWrite(thrp, LOW);
      digitalWrite(thrd, LOW);
    }
    //
    //Node Display
        Serial.print("Node = ");
        Serial.print(node);
        Serial.print("\tLeft = ");
        Serial.print(pwml);
        Serial.print("\tRight = ");
        Serial.println(pwmr);
    
    if (throwing)
    {
      PS4.setLedFlash(10, 10); // Turn on blinking
      //Yaw control
      if (PS4.getAnalogButton(L2))
      {
//        Serial.print("L2 = ");
//        Serial.print(PS4.getAnalogButton(L2));
        w = PS4.getAnalogButton(L2);
        w = (w > 200) ? 200 : w;
        w = -w;
      }
      else if (PS4.getAnalogButton(R2))
      {
//        Serial.print("R2 = ");
//        Serial.print(PS4.getAnalogButton(R2));
        w = PS4.getAnalogButton(R2);
        w = (w < -200) ? -200 : w;
      }
      else w = 0;
    }
    else
    {
      PS4.setLedFlash(0, 0); // Turn off blinking      
      pwml = 0;
      pwmr = 0;
      analogWrite(lmp, pwml);
      digitalWrite(lmd, LOW);
      analogWrite(rmp, pwmr);
      digitalWrite(rmd, LOW);
      w=pidW();      
    }
    //
    

    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117)
    {
//      Serial.print("x = ");
//      Serial.print(PS4.getAnalogHat(RightHatX));
 //      Serial.print("\ty = ");
//      Serial.println(PS4.getAnalogHat(LeftHatY));
      if(throwing)
      {
        x = (PS4.getAnalogHat(LeftHatY) - 128);
        x = -x;
        y = (PS4.getAnalogHat(RightHatX) - 128);   
        y = -y;
      }
      else
      {
        x = (PS4.getAnalogHat(RightHatX) - 128);
        x = -x;
        y = 4 * (PS4.getAnalogHat(LeftHatY) - 128);
        w = pidW();
      }
    }
    else
    {
      x = 0;
      y = 0;
    }
    invKinematics();
    pwmAndDirectionCalc();
    motion();
    //displayPidXValues();
    //displayPidWValues();
    //displayMotionValues();
  }
  else
  {
    stopp(false);
  }
}

