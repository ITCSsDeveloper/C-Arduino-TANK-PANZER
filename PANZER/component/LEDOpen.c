void LEDOpen(String led_num)
{
  if(led_num == "ALL")
  {
    digitalWrite(LED_L,HIGH);
    digitalWrite(LED_R,HIGH);
    LightStatus = "ON";
  }
  else if (led_num == "L")
  {
    digitalWrite(LED_R,LOW);
    digitalWrite(LED_L,HIGH);
    LightStatus = "L";
  }
  else if (led_num == "R")
  {
    digitalWrite(LED_L,LOW);
    digitalWrite(LED_R,HIGH);
    LightStatus = "R";
  }
  else if (led_num == "OFF")
  {
    digitalWrite(LED_L,LOW);
    digitalWrite(LED_R,LOW);
    LightStatus = "OFF";
  }
}
