void MoveAdvance(int val)
{
	LcdWrite(0,0,"MoveAdvance : ");
	int timeDelay_ADV = 75;
	if(val == 90) 
	{
		digitalWrite(motorL_L,LOW);
		digitalWrite(motorR_H,LOW);
		for(byte i = 1; i <= move_advance; i++)
		{
			TEMP = "Prograss:";
			digitalWrite(motorL_H,HIGH);
			digitalWrite(motorR_L,HIGH);
			TEMP.concat(i); 
			TEMP.concat("/"); 
			TEMP.concat(move_advance);
			LcdWrite(1,0,TEMP);
			delay(move_delay);
		}
	}
	else if(val > 90) // ต้องการไปทางขวา
	{
		val -= 90; // ลบคืน และ เอาแต่พื้นที่ทางขวา
		val /= 15; // หารองศาคือ เพื่อเอาจำนวนครั้งการหมุน

		timeDelay_ADV -= 26;
		for(byte i = 1; i <= val; i ++)
		{
			MoveAdvanceR();
			delay(timeDelay_ADV);
		}
	}
	else if (val < 90)
	{
		val /= 15; // หารองศาคือ เพื่อเอาจำนวนครั้งการหมุน 
		val -= 7; // -7 เพราะ ต้องหมุนไปทางซ้าย ทีล่ะ 15 องศา
		val = abs(val); // กลับค่าให้ไม่ติดลบ
		// ต้องการให้หมึนจาก 90 ไปที่ 45 = 45/15=3; 3-7=-4; abs(-4)=4
		// จากนั้นให้หมุนไปซ้าย 4*15 องศา จะได้ตำแหน่งที่ 45องศาพอดี
		LcdWrite(0,0,String(val));

		delay(3000);
		timeDelay_ADV -= 42;
 		for(byte i = 1; i <= val; i ++)  
		{
			MoveAdvanceL();
			delay(timeDelay_ADV);
		}
	}

	delay(100);
	ClearMove();
}

void MoveAdvanceL()
{
	for(byte i = 1; i <= move_advance; i++)
	{
		TEMP = "Prograss:";
		digitalWrite(motorL_H,HIGH);
		digitalWrite(motorL_L,LOW);

		digitalWrite(motorR_H,HIGH);
		digitalWrite(motorR_L,LOW);

		TEMP.concat(i); 
		TEMP.concat("/"); 
		TEMP.concat(move_advance);
		LcdWrite(1,0,TEMP);
		delay(move_delay);
	}
}

void MoveAdvanceR()
{
	for(byte i = 1; i <= move_advance; i++)
	{
		TEMP = "Prograss:";
		digitalWrite(motorL_H,LOW);
		digitalWrite(motorL_L,HIGH);

		digitalWrite(motorR_H,LOW);
		digitalWrite(motorR_L,HIGH);

		TEMP.concat(i); 
		TEMP.concat("/"); 
		TEMP.concat(move_advance);
		LcdWrite(1,0,TEMP);
		delay(move_delay);
	}
}