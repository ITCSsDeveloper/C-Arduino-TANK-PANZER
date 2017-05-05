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