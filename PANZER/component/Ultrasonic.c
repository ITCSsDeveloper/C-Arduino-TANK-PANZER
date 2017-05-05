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