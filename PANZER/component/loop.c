void loop() 
{ 
  LEDAuto();
  // Ultrasonic();
  // return;
  if(AutoPathEnable) { AutoPath(); }

  if (irrecv.decode(&results)) {

    CODE = DeCode(results.value);

    if(CODE != "..")
    {
      if(CODE == "UP")
      { 
        LcdWrite(0,0,"->Move UP");
        Move("UP"); 
      }
      else if (CODE == "DOWN")
      {
        LcdWrite(0,0,"->Move DOWN");
        Move("DOWN");
      }
      else if (CODE == "LEFT")
      {
        LcdWrite(0,0,"->Move LEFT");
        Move("LEFT");
      }
      else if (CODE == "RIGHT")
      {
        LcdWrite(0,0,"->Move RIGHT");
        Move("RIGHT");
      }
      else if (CODE == "*")
      {
        if(LED_Auto == true)
        {
          LcdWrite(0,0,"->Auto Light OFF");
          LcdClear("1");
          LED_Auto = false;
          LEDOpen("OFF");
        }
        else 
        {
          String TEMP;
          LcdWrite(0,0,"->Auto Light ON");
          LcdClear("1");
          TEMP.concat("Min:");
          TEMP.concat(SensorValueLimitMIN);
          LcdWrite(1,0,TEMP);
          LED_Auto = true;
        }
      }
      else if (CODE == "#")
      {
        if(LED_Auto == false)
        {
          if(LightStatus == "OFF")
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light ON");
            delay(1000);
            LEDOpen("ALL");
          }
          else if (LightStatus == "ON" || LightStatus == "R" || LightStatus == "L")
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light OFF");
            delay(1000);
            LEDOpen("OFF");
          }
        }
      }
      else if (CODE == "7")
      {
        if(LED_Auto == false)
        {
          if(LightStatus != "L")
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light L ON");
            delay(1000);
            LEDOpen("L");
          }
          else
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light L OFF");
            delay(1000);
            LEDOpen("OFF");
          }
        }
      }
      else if (CODE == "9")
      {
        if(LED_Auto == false)
        {
          if(LightStatus != "R")
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light R ON");
            delay(1000);
            LEDOpen("R");
          }
          else
          {
            LcdWrite(0,0,"->Auto Light OFF");
            LcdWrite(1,3,"Light R OFF");
            delay(1000);
            LEDOpen("OFF");
          }
        }
      }
    }
    else // ถ้าหาอะไรไม่เจอ ให้รอก่อน
    { 
      delay(100); 
    }
    
    irrecv.resume();
  } // END Reciver'
}