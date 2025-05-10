#include<LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include<Wire.h>
#include<Adafruit_Fingerprint.h>

/*蓝牙串口通信引脚 RX 0 TX 1*/
/*不能设置两个软串口通信引脚，否则会相互冲突*/
#define doorpin 8 /*使用pin8作为机械锁的控制串口*/
#define mySerial Seriall
SoftwareSerial mySerial(2,3);/*设置指纹模块软串口通信引脚 RX 2 TX 3*/
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int TrigPin=7;
int EchoPin=6;
float cm;/*超声波模块引脚*/

int code=114;/*设置密码，初始化为114*/
int p;/*设置口令*/
/*初始密钥 114*/
LiquidCrystal_I2C lcd(0x27,16,2);/*创建液晶屏对象*/

void doorOpen();/*开锁函数*/

int getFingerprintIDez();/*指纹ID识别函数*/
uint8_t readnumber(void);
uint8_t id;

void setup() {
  Serial.begin(9600);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);/*设置超声波模块引脚*/
  pinMode(doorpin,OUTPUT);
  lcd.init();/*显示屏初始化*/

  while(!Serial);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  finger.begin(57600);/*设置指纹模块波特率*/
  delay(5);
  if(finger.verifyPassword()){
    Serial.println("Found fingerprint sensor!");
  }else{
    Serial.println("Did not find fingerprint sensor.");
    {delay(1);}
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains ");Serial.print(finger.templateCount);Serial.println(" templates");
  Serial.println("Waiting for valid finger...");/*电脑和蓝牙端的检测程序*/
  Serial.println("可以输入蓝牙指令\n");
  Serial.println("操作方法：\n输入100X(密码)进行开锁\n");
  Serial.println("输入101X(原密码）X(新密码）进行更改密码\n");
  Serial.println("输入102X初始密钥进行密码初始化\n");
  Serial.println("输入103X99进行临时指纹录入\n");
}
void loop() {
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);
  cm=pulseIn(EchoPin,HIGH)*(34000.0/1000000)/2.0;
  if(1){
    lcd.backlight();/*开启背光*/
    lcd.setCursor(0,0);
    lcd.print("please enter");
    lcd.setCursor(0,1);
    lcd.print("your password.");
  }
  /*识别程序*/
    getFingerprintIDez();
    delay(50);
    while (Serial.available() > 0)
  {
    p = Serial.parseInt();
    if(Serial.read()=='X'){
      switch(p){
        case 100:/*开门程序，请务必写成100Xcode的形式*/
          while(Serial.available()>0){
          p=Serial.parseInt();
          if(p==code){
            Serial.println("开门成功\n");
            doorOpen();
            break;}
          else{
            Serial.println("密码错误\n");
            break;}
          }
       case 101:/*修改密码，请务必写成101X（旧密码）X（新密码）的形式*/
         while(Serial.available()>0){
          p=Serial.parseInt();
          if(Serial.read()=='X'&&p==code){
            p=Serial.parseInt();
            code=p;
            Serial.print("密码修改成功，现在密码为");
            Serial.println(code);break;}
          else{
            Serial.println("原密码错误\n");break;}
         }
       case 102:/*输入初始密钥使密码初始化，请务必写成102X初始密钥的形式*/
         while(Serial.available()>0){
          p=Serial.parseInt();
          if(p==114){
            code=114;
            Serial.println("密码已重新初始化！");break;}
          else{Serial.println("初始密钥错误\n");break;}
         }
       case 103:/*临时记录一个指纹便于测试*/
       while(Serial.available()>0){
        p=Serial.parseInt();
        if(p==99){
         Serial.println("Ready to enroll a fingerprint!");
         Serial.println("lease type in the ID#(from 1 to 127)you want to save this finger as...");
         id=readnumber();
         if(id==0) return;
         Serial.print("Enrolling ID");
         Serial.println(id);
         while(!getFingerprintEnroll());
         break;
        }
        else break;
       case 104:/*清除所有指纹*/
       for(uint8_t num=2;num<=127;num++){
        deleteFingerprint(num);
       }
       Serial.println("已清除所有指纹！");
       break;
    }
    }
   }
  }
}
uint8_t getFingerprintID(){
  uint8_t p=finger.getImage();
  switch(p){
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }/*维修&排除异常情况*/
  p=finger.image2Tz();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }/*排除错误情况*/
  p=finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
  Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  Serial.println("Communication error");
  return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
  Serial.println("Did not find a match");
  return p;
  } else {
  Serial.println("Unknown error");
  return p;
  }
   
}

int getFingerprintIDez(){
  uint8_t p=finger.getImage();
  if(p!=FINGERPRINT_OK) return -1;
  p=finger.image2Tz();
  if(p!=FINGERPRINT_OK) return -1;
  p=finger.fingerFastSearch();
  if(p!=FINGERPRINT_OK) return -1;

  if(finger.confidence>=70){
    doorOpen();
  }/*标定了指纹检测的确信值，一般来说在60-80之间比较适合，越高越安全，也越不容易开锁。*/
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Not Welcome.");
    lcd.setCursor(0,1);
    lcd.print("or use code...");
    delay(2000);
    lcd.clear();
  }
}

uint8_t readnumber(void){
  uint8_t num=0;
  while(num==0){
    while(!Serial.available());
    num=Serial.parseInt();
  }
  return num;
}
uint8_t getFingerprintEnroll(){
  int p=-1;
  Serial.print("Waiting for valid finger to enroll as...");Serial.println(id);
  while (p != FINGERPRINT_OK) {
  p = finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
    Serial.println(".");
    break;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    break;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    break;
  default:
    Serial.println("Unknown error");
    break;
  }
  }
  p = finger.image2Tz(1);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
}
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
  p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("lace same finger again");
  while (p != FINGERPRINT_OK) {
  p = finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
  case FINGERPRINT_NOFINGER:
    Serial.print(".");
    break;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    break;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    break;
  default:
    Serial.println("Unknown error");
    break;
  }
  }
   p = finger.image2Tz(2);
  switch (p) {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }
  Serial.print("Creating model for #");  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  Serial.println("rints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  Serial.println("Communication error");
  return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
  Serial.println("Fingerprints did not match");
  return p;
  } else {
  Serial.println("Unknown error");
  return p;
  }  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
  Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  Serial.println("Communication error");
  return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
  Serial.println("Could not store in that location");
  return p;
  } else if (p == FINGERPRINT_FLASHERR) {
  Serial.println("Error writing to flash");
  return p;
  } else {
  Serial.println("Unknown error");
  return p;
  }
}
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }
  return p;
} 
void doorOpen(){
  digitalWrite(doorpin,HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  delay(2000);
  digitalWrite(doorpin,LOW);
}/*开锁函数*/
