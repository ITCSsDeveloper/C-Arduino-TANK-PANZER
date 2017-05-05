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