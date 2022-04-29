#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
Servo myservo;  
int pos = 0;                  // มุน Servo
const int TrigPin = 12;       // Trig -> 12
const int EchoPin = 11;       // Echo -> 11
int detected = 0;             // ตัวแปรตรวจจับ
uint8_t heart[8] = {          //ทำรูปหัวใจ
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
};
//=========================================================================
void setup() {
  Serial.begin(9600);         // อ่านค่าผ่าน Serial Monitor
  myservo.attach(9);          // ขา D11 ต่อกับ Servo
  myservo.write(pos);         // Servo หมุนที่จุดเริ่มต้น (0 องศา)
  pinMode(TrigPin, OUTPUT);   // TrigPin เป็น OUTPUT
  pinMode(EchoPin, INPUT);    // EchoPin เป็น INPUT
  lcd.init();                 // Print a message to the LCD.
  lcd.backlight();            // เปิดไฟที่ lcd
}
//=========================================================================
void loop() {                      
  digitalWrite(TrigPin, LOW);           // ส่งคลื่นเสียง 0
  delayMicroseconds(2);                 // หน่วงเวลา
  digitalWrite(TrigPin, HIGH);          // ส่งคลื่นเสียง 1
  delayMicroseconds(5);                 // หน่วงเวลา
  digitalWrite(TrigPin, LOW);           // ส่งคลื่นเสียง 0
  long duration = pulseIn(EchoPin, HIGH);    // ระยะเวลาในการเดินทางของเสียง
  long cm = msTocm(duration); // แปลงค่าเป็นระยะทาง
  Serial.print(cm);                     // แสดงค่าค่าผ่าน Serial Monitor
  Serial.print("cm");                   // แสดงค่าค่าผ่าน Serial Monitor
  Serial.println();                     // แสดงค่าค่าผ่าน Serial Monitor
  if((cm<10)&&(detected==1)){           // ถ้าระยะน้อยกว่า 10 และยังไม่เคยตรวจจับพบวัตถุ
    detected = 0;                       // ตรวจจับพบวัตถุแล้ว
    for (int pos=0; pos<=90; pos+=1) {      // Servo หมุนทีละ 1 องศา สูงสุดที่ 80 องศา
      myservo.write(pos);               // Servo หมุน
      delay(3);                         // หน่วงเวลา
    }
    lcd.createChar(3, heart);           // สร้างรูปหัวใจ
    lcd.setCursor(0,0);                 // ตั้งตำแหน่งที่จะปริ้น
    lcd.print("====================");         // ปริ้นข้อความ
    lcd.setCursor(5,1);                 // ตั้งตำแหน่งที่จะปริ้น
    lcd.print("Thank you\x03");         // ปริ้นข้อความ
    lcd.setCursor(1,2);                 // ตั้งตำแหน่งที่จะปริ้น
    lcd.print("Take care yourself");    // ปริ้นข้อความ
    lcd.setCursor(0,3);                 // ตั้งตำแหน่งที่จะปริ้น
    lcd.print("====================");         // ปริ้นข้อความ
    for (int pos=80; pos>=0; pos-=1) {  // Servo หมุนทีละ -1 องศา ต่ำสุดที่ 0 องศา
      myservo.write(pos);               // Servo หมุน
      delay(3);                         // หน่วงเวลา
    }
  }
  if(cm>=10){                           // ถ้าระยะมากกว่า 10
    detected = 1;                       // ยังไม่เคยตรวจจับพบวัตถุ
    myservo.write(0);                   // หมุน servo กลับมาที่เดิม
    delay(100);                         // หน่วงเวลา 100 ms
    lcd.clear();                        // clear หน้าจอ lcd
  }                                     
  delay(100);                           // หน่วงเวลา
}
//=========================================================================
long msTocm(long ms) {            //ฟังก์ชันแปลงเวลาเป็นระยะทาง
// The speed of sound is 340 m/s or 29 ms per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return ms / 29 / 2;
}