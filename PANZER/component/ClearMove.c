void ClearMove()
{
  digitalWrite(motorL_H,LOW); digitalWrite(motorR_H,LOW);
  digitalWrite(motorL_L,LOW); digitalWrite(motorR_L,LOW);
}

// ล้างค่าที่ส่งไป IC ขัขมอเตอร์