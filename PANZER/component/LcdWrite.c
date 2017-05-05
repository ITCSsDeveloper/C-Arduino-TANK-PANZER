void LcdWrite(int line,int point,String Message)
{
  lcd.setCursor(0,line);
  lcd.print("                ");
  lcd.setCursor(point,line);
  lcd.print(Message);
}
