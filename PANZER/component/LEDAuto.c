void LEDAuto()
{
  if(LED_Auto)
  {
    if(analogRead(LightSensor) > SensorValueLimitMIN) // ถ้าค่าแสงน้อยกว่าที่ระบุไว้ ให้เปิดไฟ
    {
      LEDOpen("ALL");
    }
    else 
    {
      LEDOpen("OFF");
    }
  }
}