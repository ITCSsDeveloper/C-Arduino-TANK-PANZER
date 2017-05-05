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

