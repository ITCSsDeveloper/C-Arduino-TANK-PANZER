int DistanceRow1[13];
// 0 15 30 45 60 75 |90| 105 120 135 150 165 180
// 0  1  2  3  4  5   6   7   8   9   10  11  12

bool flag = true;
void AutoPath()
{	
	if(flag) { CheckRoute(); flag =false; return; }
	if(false) { MoveAdvance(0); delay(1000); MoveAdvance(90); delay(1000); MoveAdvance(180); delay(1000); return; }
	if(Ultrasonic() > lengthLimit)
	{
		MoveAdvance(90);  // ถ้าทางข้างหน้ามีทางไป ก็ให้ไปซ่ะ
	}
	else
	{
		CheckRoute(); // ค้นหาเส้นทางอื่นที่เป็นไปได้
		byte Route = DecidingRoute(); // ทำการคำนวณเส้นทางที่เหมาะแก่การไป
		Route *= 15; // * องศาการหมุน 
		MoveAdvance(Route); // ทำการหันทิศทางไปยังเป้าหมาย
	}
}

void CheckRoute() // ค้นหาเส้นทาง
{
	LcdWrite(0,0,"CheckRoute");
	byte value = 0;
	ServoCtrl(0);
	delay(100);
	for(byte i = 0 ; i <= 12; i++)
	{
		ServoCtrl(value);
		
		if(i == 0 || i == 6 || i == 12) 
		{ 
			DistanceRow1[i] = Ultrasonic(); 
		}
		else
		{
			DistanceRow1[i] = 0;
		}

		if(false) // อัลกอริทึมเดิม เอาทุกจุด
		{
			 if(i == 2 || i == 3 || i == 9 || i == 10) 
			{ 
				DistanceRow1[i] = 0;
			}
			else
			{
				DistanceRow1[i] = Ultrasonic(); 
			}
		}

		value += 15;

		TEMP = "i=";
		TEMP.concat(i); 
		TEMP.concat(" var="); 
		TEMP.concat(DistanceRow1[i]);
		LcdWrite(1,0,TEMP);
	}
	ServoCtrl(90);
	delay(100);
	Buzzer(1);
}

byte DecidingRoute()
{	
	LcdWrite(0,0,"DecidingRoute");
	int max = 0;
	byte NumOfMax = 0;
	byte duplicate = 0;
	byte NearValue[13];
	for(byte i = 0 ; i <= 12; i++)
	{
		Serial.print(DistanceRow1[i]);
		Serial.print(" ");

		if(DistanceRow1[i] > max) // มากกว่า MAX 
		{ 
			max = DistanceRow1[i]; 
			NumOfMax = i;  
			duplicate = 0; // ล้างค่า เพราะมากกว่า 
		}
		else if(DistanceRow1[i] == max)
		{
			NumOfMax = i;
			NearValue[0] = max;
			duplicate++;
			NearValue[duplicate] = i; //เก็บค่าตำแหน่งซ้ำไว้
		}

		TEMP = "";
		TEMP.concat(i); 
		TEMP.concat(" Max="); 
		TEMP.concat(max);
		TEMP.concat(" Cm="); 
		TEMP.concat(NumOfMax);
		delay(100);
		LcdWrite(1,0,TEMP);
	}

	if (duplicate != 0) // มีหลายเส้นทาง
	{
		duplicate += 1; // เพิ่มค่า เพื่อไม่สับสนเฉยๆ
		if(duplicate == 2) // มี 2 เส้นทาง ที่เท่ากัน
		{
			// ถ้า ขวา น้อยกว่า ซ้าย ให้ใช้ค่าซ้าย
			if(DistanceRow1[NumOfMax+1] < DistanceRow1[NumOfMax-2])
			{
				NumOfMax -= 1;
			}
		}
		else if(duplicate == 3)
		{
			NumOfMax -= 1;
		}
		else if (duplicate > 3)
		{
			if(duplicate%2 == 0) // mod2 ==0 เป็นเลขคู่
			{
				NumOfMax /= 2;
			}
			else
			{
				NumOfMax /=2;
				NumOfMax += 1;
			}
		}
	}
	Buzzer(1);
	TEMP = "";
	TEMP.concat("BestPath="); 
	TEMP.concat(NumOfMax);
	LcdWrite(1,0,TEMP);
	delay(250);
	return NumOfMax;
}