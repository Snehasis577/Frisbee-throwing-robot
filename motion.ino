//       motor configuration:
//---------------------------------------------
//     | motor2(fld,fla)   |   motor1(frd,fra) |
//---------------------------------------------
//     | motor3(bld,bla)   |   motor4(brd,bra) |
//---------------------------------------------

// motion variable variable
int  d1, d2, d3, d4;
float s1, s2, s3, s4; // speed variables of motor
int sp = 1;
//Base Motor Pins
byte frd = 39, fld = 37, bld = 23, brd = 31, fra = 12, fla = 11, bla = 4, bra = 8;
//

void invKinematics()//calculation for speed of motors 
{
      s1 = sp * ((a11 * x) + (a12 * y) + (a13 * w));
      s2 = sp * ((a21 * x) + (a22 * y) + (a23 * w));
      s3 = sp * ((a31 * x) + (a32 * y) + (a33 * w));
      s4 = sp * ((a41 * x) + (a42 * y) + (a43 * w));
}

void pwmAndDirectionCalc()
{
  //direction calculations   
    d1=s1<0?0:1;
    d2=s2<0?0:1;
    d3=s3<0?1:0;
    d4=s4<0?1:0;
    
  //magnitude 
    s1=abs(s1);
    s2=abs(s2);
    s3=abs(s3);
    s4=abs(s4);
     
}

void motion()/// commands the motors to drive
{

  digitalWrite(frd, d1);
  digitalWrite(fld, d2);
  digitalWrite(brd, d4);
  digitalWrite(bld, d3);
  analogWrite(fra, s1);
  analogWrite(fla, s2);
  analogWrite(bla, s3);
  analogWrite(bra, s4);
}
void stopp(bool flag)
{
  digitalWrite(frd, d1);
  digitalWrite(fld, d2);
  digitalWrite(brd, d4);
  digitalWrite(bld, d3);
  if(!flag) s1=s2=s3=s4=0;
  for(int i=0;i<500;i++)
  {
    s1-=10;
    s1 = (s1<=0)?0:s1;
    s2-=10;
    s2 = (s2<=0)?0:s2;
    s3-=10;
    s3 = (s3<=0)?0:s3;
    s4-=10;
    s4 = (s4<=0)?0:s4;
    if(s1==0 || s2==0 || s3==0 || s4==0) { s1=s2=s3=s4=0; break;}
    analogWrite(fra, s1);
    analogWrite(fla, s2);
    analogWrite(bla, s3);
    analogWrite(bra, s4);
  }
    analogWrite(fra, 0);
    analogWrite(fla, 0);
    analogWrite(bla, 0);
    analogWrite(bra, 0);
}

void displayMotionValues()
{
      Serial.print("x=");
      Serial.print(x);
      Serial.print(" y=");
      Serial.print(y);
      Serial.print(" w=");
      Serial.print(w);
      Serial.print(" s1=");

      Serial.print(s1);
      Serial.print(" s2=");
      Serial.print(s2);
      Serial.print(" s3=");
      Serial.print(s3);
      Serial.print(" s4=");
      Serial.print(s4);
      Serial.print(" d1=");
      Serial.print(d1);
      Serial.print(" d2=");
      Serial.print(d2);
      Serial.print(" d3=");
      Serial.print(d3);
      Serial.print(" d4=");
      Serial.println(d4);
}
void motion_check()/// checks the order of motors
{
  Serial.println("forward right");
  digitalWrite(frd, 1);
  analogWrite(fra, 125);
  delay(1000);
  digitalWrite(frd, 1);
  analogWrite(fra, 0);
  delay(1000);
  digitalWrite(frd, 0);
  analogWrite(fra, 125);
  delay(1000);
  digitalWrite(frd, 0);
  analogWrite(fra, 0);
  Serial.println("forward left");
  digitalWrite(fld, 1);
  analogWrite(fla, 125);
  delay(1000);
  digitalWrite(fld, 1);
  analogWrite(fla, 0);
  delay(1000);
  digitalWrite(fld, 0);
  analogWrite(fla, 125);
  delay(1000);
  digitalWrite(fld, 0);
  analogWrite(fla, 0);
  delay(1000);

  Serial.println("backward right");
  digitalWrite(brd, 1);
  analogWrite(bra, 125);
  delay(1000);
  digitalWrite(brd, 1);
  analogWrite(bra, 0);
  delay(1000);
  digitalWrite(brd, 0);
  analogWrite(bra, 125);
  delay(1000);
  digitalWrite(brd, 0);
  analogWrite(bra, 0);
  delay(1000);

  Serial.println("backward left");
  digitalWrite(bld, 1);
  analogWrite(bla, 125);
  delay(1000);
  digitalWrite(bld, 1);
  analogWrite(bla, 0);
  delay(1000);
  digitalWrite(bld, 0);
  analogWrite(bla, 125);
  delay(1000);
  digitalWrite(bld, 0);
  analogWrite(bla, 0);
  delay(1000);

}
