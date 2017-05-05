void Buzzer(byte time)
{
	if(Buzzer_Enable)
	{
		for(byte i = 1 ; i <= time ; i++) {
			digitalWrite(buzzer_2,HIGH); delay(100);
			digitalWrite(buzzer_2,LOW);  delay(100);
		}
	}
}

// ส่งสียงร้องตามจำนวนที่กรอกเข้ามา