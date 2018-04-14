
//imu read function 

float getImuValue()
{

  int i = 0;

  for(int j=0;j<40;j++)
  {
    buff[j]=0;
  }

//token request
Serial1.write("#f");
 delay(35);   //waiting for arbitrary time of around 90ms after which imu sends data packet

int f=1; //flag variable 

while(1)
{ 
while(Serial1.available())
{
  char c=Serial1.read();
  buff[i++]=c;
  f=0;
}

if(f==0)
{
  break;
}
}
String str=buff;
 yaw_string=str.substring(5,str.indexOf(","));//breaking the sting yaw
  pitch_string=str.substring(str.indexOf(",")+1,str.lastIndexOf(","));
  roll_string=str.substring(str.lastIndexOf(",")+1);

float yaw=::atof(yaw_string.c_str());//change into char array then into float
float pitch=::atof(pitch_string.c_str());
float roll=::atof(roll_string.c_str());

//Serial.println();
//Serial.print("yaw : ");
//Serial.println(yaw);
//Serial.print("pitch : ");
//Serial.println(pitch);
//Serial.print("roll : ");
//Serial.println(roll);

 return yaw;
}


