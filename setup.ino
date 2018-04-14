void ps4_setup() 
{
 
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
}

//motor setup.......
void motorSetup()
{
  pinMode(fra, OUTPUT);
  pinMode(frd, OUTPUT);
  pinMode(fla, OUTPUT);
  pinMode(fld, OUTPUT);
  pinMode(bra, OUTPUT);
  pinMode(brd, OUTPUT);
  pinMode(bla, OUTPUT);
  pinMode(bld, OUTPUT);
  pinMode(lmp, OUTPUT);
  pinMode(lmd, OUTPUT);
  pinMode(rmp, OUTPUT);
  pinMode(rmd, OUTPUT);
  pinMode(lna, OUTPUT);
  pinMode(lnd, OUTPUT);
  pinMode(thrp, OUTPUT);
  pinMode(thrd, OUTPUT);  
}

// imu setup function
void imusetup() 
{
  delay(2000);
  Serial1.begin(57600);
  Serial1.write("#o0");

  for(int i=0;i<10;i++)//stabilization of imu values
  {
  comp_ref_yaw=getImuValue();
  Serial.println(comp_ref_yaw);
  }
  if(comp_ref_yaw>0)//error in intial degrees
  ref_yaw=comp_ref_yaw;
  else ref_yaw=comp_ref_yaw;
  
}
