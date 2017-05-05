void Move(String command)
{
  if(command == "UP") 
  { 
    digitalWrite(motorL_L,LOW);
    digitalWrite(motorR_H,LOW);

    for(byte i = 1; i <= move_duration; i++)
    {
      TEMP = "Prograss:";
      digitalWrite(motorL_H,HIGH);
      digitalWrite(motorR_L,HIGH);
      TEMP.concat(i); 
      TEMP.concat("/"); 
      TEMP.concat(move_duration);
      LcdWrite(1,0,TEMP);
      delay(move_delay);
    }
  }
  else if(command == "DOWN") 
  {
    for(byte i = 1; i <= move_duration; i++)
    {
      TEMP = "Prograss:";
      digitalWrite(motorL_H,LOW);
      digitalWrite(motorL_L,HIGH);

      digitalWrite(motorR_H,HIGH);
      digitalWrite(motorR_L,LOW);

      TEMP.concat(i); 
      TEMP.concat("/"); 
      TEMP.concat(move_duration);
      LcdWrite(1,0,TEMP);
      delay(move_delay);
    }

  }
  else if(command == "RIGHT") 
  {
    for(byte i = 1; i <= degree; i++)
    {
      TEMP = "Prograss:";
      digitalWrite(motorL_H,LOW);
      digitalWrite(motorL_L,HIGH);

      digitalWrite(motorR_H,LOW);
      digitalWrite(motorR_L,HIGH);

      TEMP.concat(i); 
      TEMP.concat("/"); 
      TEMP.concat(move_duration);
      LcdWrite(1,0,TEMP);
      delay(move_delay);
    }
  }
  else if(command == "LEFT") 
  {
    for(byte i = 1; i <= degree; i++)
    {
      TEMP = "Prograss:";
      digitalWrite(motorL_H,HIGH);
      digitalWrite(motorL_L,LOW);

      digitalWrite(motorR_H,HIGH);
      digitalWrite(motorR_L,LOW);

      TEMP.concat(i); 
      TEMP.concat("/"); 
      TEMP.concat(move_duration);
      LcdWrite(1,0,TEMP);
      delay(move_delay);
    }
  }
  
  ClearMove();
}