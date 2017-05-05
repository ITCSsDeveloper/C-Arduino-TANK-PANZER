#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <Servo.h>

byte LightSensor = 0 ; // Analog
// LCD USE AnalogPin 4,5
Servo myservo;

byte motorL_H = 2;
byte motorL_L = 3;
byte motorR_H = 4;
byte motorR_L = 5;
byte buzzer_1 = 6; //ไม่ใช้แล้ว
byte buzzer_2 = 7;
byte LED_L = 8;
byte LED_R = 9;
byte ServoPIN = 10;
byte RECV_PIN = 13; // Remote

byte ultrasonic_trig = 11;
byte ultrasonic_echo = 12;

// LCD
#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);

// Remote
IRrecv irrecv(RECV_PIN);
decode_results results;

// Recver form irrecv.decode
String CODE; // String for irrecv
String TEMP; // String for concat

// StartUP Check
bool StartUP_Work = true;
String LCDStatus = "";

// LED Sensor
String LightStatus;
bool LED_Auto = true;
int SensorValueLimitMIN = 600; // ค่ายิ่งมาก แสงยิ่งน้อย

// Movement
byte move_delay = 1;  // เวลาในการหมุนของ มอเตอร์ // น้อย = เร็ว , มาก =  ช้า
byte move_duration = 10; // ใน 1 command ให้ทำกี่รอบ  ** ไม่ใช้
byte move_advance = 1;
byte degree = 10; // การเลี้ยว

int lengthLimit = 17; // จำกัด ระยะการเดินทาง // ถ้าน้อยกว่า แสดงว่าไปไม่ได้

bool AutoPathEnable = false;

bool Buzzer_Enable = false; // ใช้เสียง Buzzer 

int SerialRate = 250000;

bool DebugMode = false;


void setup() 
{
  //Serial.begin(SerialRate);

  pinMode(motorL_L,OUTPUT);
  pinMode(motorL_H,OUTPUT);
  pinMode(motorR_L,OUTPUT);
  pinMode(motorR_H,OUTPUT);
  pinMode(buzzer_1,OUTPUT);
  pinMode(buzzer_2,OUTPUT);
  pinMode(LED_L,OUTPUT);
  pinMode(LED_R,OUTPUT);

  irrecv.enableIRIn();

  lcd.begin (16,2); 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); 

  digitalWrite(buzzer_1,LOW);
  digitalWrite(buzzer_2,LOW);

  myservo.attach(ServoPIN);

  Serial.println("setup");
  ServoCtrl(0);Serial.println("setup");
  ServoCtrl(90);Serial.println("setup");

}

void ServoCtrl (byte degree)
{
  	degree = abs(degree - 180); // -180 ติด Servo ผิดด้าน = = 
	for(int i =0 ;i <= 180 ; i++) 
	{
		myservo.write(degree);
		LEDAuto();
		delay(1);  
	}   
}

// รับค่า องศา ที่ต้องการเข้ามา
// แล้วจะปั่น loop เพื่อยิง องศาให้ตรง (กำหนด 180 เพราะเป็นค่าที่พอดีแล้ว)
// เพื่อลดการ delay ใช้ค่า 180 ดีสุด

void LcdWrite(int line,int point,String Message)
{
  lcd.setCursor(0,line);
  lcd.print("                ");
  lcd.setCursor(point,line);
  lcd.print(Message);
}

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

void ClearMove()
{
  digitalWrite(motorL_H,LOW); digitalWrite(motorR_H,LOW);
  digitalWrite(motorL_L,LOW); digitalWrite(motorR_L,LOW);
}

// ล้างค่าที่ส่งไป IC ขัขมอเตอร์

const char * DeCode(int input)
{
  switch(input)
  {
    case 16736925 : return "UP";    break;
    case 16754775 : return "DOWN";  break;
    case 16720605 : return "LEFT";  break;
    case 16761405 : return "RIGHT"; break;
    case 16712445 : return "OK";    break;
    case 16738455 : return "1";     break;
    case 16750695 : return "2";     break;
    case 16756815 : return "3";     break;
    case 16724175 : return "4";     break;
    case 16718055 : return "5";     break;
    case 16743045 : return "6";     break;
    case 16716015 : return "7";     break;
    case 16726215 : return "8";     break;
    case 16734885 : return "9";     break;
    case 16730805 : return "0";     break;
    case 16728765 : return "*";     break;
    case 16732845 : return "#";     break;
    default       : return "..";    break;
  }
}

// แปรรหัส IR ไปเป็น String 

int UltraDuration;
byte Ultrasonic()
{
	delay(1);
	UltraDuration = 0;
	pinMode(ultrasonic_trig, OUTPUT);
	digitalWrite(ultrasonic_trig, LOW);
	delayMicroseconds(2);
	digitalWrite(ultrasonic_trig, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultrasonic_trig, LOW);
	pinMode(ultrasonic_echo, INPUT);
	UltraDuration = pulseIn(ultrasonic_echo, HIGH);

	UltraDuration = (UltraDuration / 29 / 2);

	if(UltraDuration <= 0) { UltraDuration = 0; }
	if(UltraDuration >= 100) { UltraDuration = 0; }

//	LcdWrite(1,12,String(UltraDuration)); // For monitor

	delay(1);
	return UltraDuration; //microsecondsToCentimeters
}

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


void StartUp(bool Enable)
{
 
}

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

