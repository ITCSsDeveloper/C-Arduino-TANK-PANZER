void LcdClear(String line)
{
  if(line == "0") 
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
  }
  else if(line == "1")
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
  else if(line == "ALL")
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("                ");
  }
}
