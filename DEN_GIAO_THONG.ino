#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_AUTH_TOKEN  "BcuTlLacvqprx4lcHcmIsyuKegQBeE3S"
#define x1 5
#define d1 19
#define v1 18
#define xb1 17
#define db1 16

#define x2 21
#define d2 23
#define v2 22
#define xb2 32
#define db2 13
long time1, time2, time3;
long Td = 8000, Tx = 5000, Tv = 3000; //tổng Tx + Tv phải = Td
long Td1, Tx1, Tv1; //tổng Tx + Tv phải = Td
#define Mode 4
#define tang 2
#define giam 15

#define cambien 34
int s = 0;
#include <ShiftRegister74HC595.h>
#define SDI 14
#define SCLK 12
#define LOAD 27
#define DIGITS 2


int i = 0, j = 0;
// create shift register object (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr (DIGITS, SDI, SCLK, LOAD);




//led tuyen 2
#define SDI1 25
#define SCLK1 26
#define LOAD1 33
#define DIGITS1 2
// create shift register object (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr1 (DIGITS1, SDI1, SCLK1, LOAD1);
int value, digit1, digit2, digit3, digit4;
uint8_t  digits[] = {B11000000, //0
                     B11111001, //1
                     B10100100, //2
                     B10110000, //3
                     B10011001, //4
                     B10010010, //5
                     B10000010, //6
                     B11111000, //7
                     B10000000, //8
                     B10010000 //9
                    };

int dem1 = 0, dem2, dem3;
int dem11 = 0, dem22, dem33;

#include <WiFi.h>
char ssid[] = "tuiladahi";  ///tên wifi
char pass[] = "15092001";   /////Mật khẩu wifi
WiFiServer server(80);
IPAddress local_IP(192, 168, 1, 123);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
int gpio16Value;
String header;
String LED_ONE_STATE = "off";

//THỜI GIAN THỰC NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int setgio = 23; //set giờ hoạt động của chế độ đèn giao thông

int chedo, denvang, len, xuong;
BLYNK_WRITE(V7)
{
  len = param.asInt(); // assigning incoming value from pin V1 to a variable
}
BLYNK_WRITE(V10)
{
  xuong = param.asInt(); // assigning incoming value from pin V1 to a variable
}
BLYNK_WRITE(V8)
{
  denvang = param.asInt(); // assigning incoming value from pin V1 to a variable

}
long timet;
int currentMinute,currentHour;
int currentSecond;
int ttd1, ttd2, ttx1, ttx2, ttv1, ttv2, ttxb1, ttxb2, ttdb1, ttdb2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(x1, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(v1, OUTPUT);
  pinMode(db1, OUTPUT);
  pinMode(xb1, OUTPUT);
  pinMode(x2, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(db2, OUTPUT);
  pinMode(xb2, OUTPUT);
  Td1 = Td;
  Tx1 = Tx;
  Tv1 = Tv;
  dem1 = Tx;
  dem2 = Tv;
  dem3 = Td;
  dem11 = Tx;
  dem22 = Tv;
  dem33 = Td;
  time1 = millis();
  pinMode(tang, INPUT_PULLUP);
  pinMode(giam, INPUT_PULLUP);
  pinMode(Mode, INPUT_PULLUP);
  pinMode(cambien, INPUT_PULLUP);
  ///////////////////////////////////////////
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk-server.com", 8080);
  server.begin();

  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +7 = 25200
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);
  Blynk.virtualWrite(V1, 0);  // phân loại dữ liệu, vd nhãn là tag và 10 là data (tag:10) {v1, 0)
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V6, 0);
  Blynk.virtualWrite(V7, 0);
  Blynk.virtualWrite(V8, 0);
  Blynk.virtualWrite(V10, 0);
  timet = millis();
}



void loop() {
  /////Lấy dữ liệu thời gian
  timeClient.update();//cập nhật thời gian
  time_t epochTime = timeClient.getEpochTime();//biến lưu thời gian
  String formattedTime = timeClient.getFormattedTime(); //định dạng thời gian HH:MM:SS
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;  //lấy dữ liệu ngày
  currentHour = timeClient.getHours(); //lấy dữ liệu giờ
  currentMinute = timeClient.getMinutes();  //lấy dữ liệu phút
  currentSecond = timeClient.getSeconds();   //lấy giữ liệu giây
  timet = millis();
  Serial.println(String(currentHour) + " : " + String(currentMinute));
  
  
  Blynk.virtualWrite(V9, (Td + Tx + Tv) / 1000); // milli giây/1000 = giây

  if (digitalRead(Mode) == 0) {
    i = !i;
    dem1 = Tx;
    dem2 = Tv;
    dem3 = Td;
    dem11 = Tx1;
    dem22 = Tv1;
    dem33 = Td1;
    delay(300);
  }
  if (i == 0) {
    if (digitalRead(tang) == 0 || denvang == 1) {
      j = 1;
      denvang = 1;
      Blynk.virtualWrite(V8, 1);
      Serial.println("DA NHAN NUT BAT DEN VANG");
    }
    if (digitalRead(giam) == 0 || denvang == 0) {
      j = 0;
      denvang = 0;
      Blynk.virtualWrite(V8, 0);
    }
  }

  if (currentHour >= setgio || j == 1) { //Sau 6h chiều đèn sẽ chỉ nháy vàng)
     
    Blynk.virtualWrite(V2, 1);// gui du lieu len sv
    Blynk.virtualWrite(V5, 1);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V6, 0);
    sr.setAllHigh(); // set all pins LOW
    sr1.setAllHigh(); // set all pins LOW
    digitalWrite(d2, LOW);
    digitalWrite(x2, LOW);
    digitalWrite(v2, HIGH);
    digitalWrite(d1, LOW);
    digitalWrite(x1, LOW);
    digitalWrite(v1, HIGH);
    delay(300);
    digitalWrite(d2, LOW);
    digitalWrite(x2, LOW);
    digitalWrite(v2, LOW);
    digitalWrite(d1, LOW);
    digitalWrite(x1, LOW);
    digitalWrite(v1, LOW);
    digitalWrite(db2, LOW);
    digitalWrite(xb2, LOW);
    digitalWrite(db1, LOW);
    digitalWrite(xb1, LOW);
    delay(300);
  }
  if (i == 1 ) {
    digitalWrite(d2, HIGH);
    digitalWrite(x2, HIGH);
    digitalWrite(v2, HIGH);
    digitalWrite(d1, HIGH);
    digitalWrite(x1, HIGH);
    digitalWrite(v1, HIGH);
    if (digitalRead(tang) == 0) {
      Td = Td + 2000; Tx = Tx + 1000; Tv = Tv + 1000;
      Td1 = Td1 + 2000; Tx1 = Tx1 + 1000; Tv1 = Tv1 + 1000;
      showNumber((Td + Tx + Tv) / 1000);
      showNumber2((Td + Tx + Tv) / 100000);
      delay(300);
    }
    if (digitalRead(giam) == 0) {
      Td = Td - 2000; Tx = Tx - 1000; Tv = Tv - 1000;
      Td1 = Td1 - 2000; Tx1 = Tx1 - 1000; Tv1 = Tv1 - 1000;
      showNumber((Td + Tx + Tv) / 1000);
      showNumber2((Td + Tx + Tv) / 100000);
      delay(300);
    }

  }
  if (len == 1) {
    Td = Td + 2000; Tx = Tx + 1000; Tv = Tv + 1000;
    Td1 = Td1 + 2000; Tx1 = Tx1 + 1000; Tv1 = Tv1 + 1000;
    showNumber((Td + Tx + Tv) / 1000);
    showNumber2((Td + Tx + Tv) / 100000);
    Blynk.virtualWrite(V7, 0);
    len = 0;
  }
  if (xuong == 1) {
    Td = Td - 2000; Tx = Tx - 1000; Tv = Tv - 1000;
    Td1 = Td1 - 2000; Tx1 = Tx1 - 1000; Tv1 = Tv1 - 1000;
    showNumber((Td + Tx + Tv) / 1000);
    showNumber2((Td + Tx + Tv) / 100000);
    Blynk.virtualWrite(V10, 0);
    xuong = 0;
  }
  if (i == 0 && currentHour < setgio && j == 0) { //Trước 6 h tối đèn giao thôgn bình thường
    //tuyến 2
    if (millis() - time1 <= Td1) {
      digitalWrite(d2, HIGH);
      digitalWrite(xb2, HIGH);
      digitalWrite(db2, LOW);
      
      ////////////////////////////////////////////
      ttx2 = 0;
      ttd2 = 1;
      ttv2 = 0;
      ttdb2 = 0;
      ttxb2 = 1;
      dem11 = Tx;
      dem22 = Tv;
      if (millis() - time3 >= 1000) {
        dem33 = (dem33 - 1000);
        showNumber2(dem33 / 1000);
        
        time3 = millis();
      }
    }
    else {
      digitalWrite(d2, LOW);
    }
    if (millis() - time1 > Td1 && millis() - time1 <= Td1 + Tx1) {
      digitalWrite(x2, HIGH);
      digitalWrite(db2, HIGH);
      digitalWrite(xb2, LOW);
      ttx2 = 1;
      ttd2 = 0;
      ttv2 = 0;
      ttdb2 = 1;
      ttxb2 = 0;
      dem22 = Tv;
      dem33 = Td;
      if (millis() - time3 >= 1000) {
        dem11 = (dem11 - 1000);
        showNumber2(dem11 / 1000);
        
        time3 = millis();
      }
    }
    else {
      digitalWrite(x2, LOW);
    }
    if (millis() - time1 > Td1 + Tx1 && millis() - time1 <= Td1 + Tx1 + Tv1) {
      digitalWrite(v2, HIGH);
      digitalWrite(db2, HIGH);
      digitalWrite(xb2, LOW);
      ttx2 = 0;
      ttd2 = 0;
      ttv2 = 1;
      ttdb2 = 1;
      ttxb2 = 0;
      dem11 = Tx;
      dem33 = Td;
      if (millis() - time3 >= 1000) {
        dem22 = (dem22 - 1000);
        showNumber2(dem22 / 1000);
        time3 = millis();
      }
    }
    else {
      digitalWrite(v2, LOW);
    }


    //tuyến 1
    if (millis() - time1 <= Tx) {
      digitalWrite(x1, HIGH);
      digitalWrite(db1, HIGH);
      digitalWrite(xb1, LOW);
      ttx1 = 1;
      ttd1 = 0;
      ttv1 = 0;
      ttxb1 = 0;
      ttdb1 = 1;
      dem2 = Tv;
      dem3 = Td;
      if (millis() - time2 >= 1000) {
        dem1 = (dem1 - 1000);
        showNumber(dem1 / 1000);
        
        time2 = millis();
      }
    }
    else {
      digitalWrite(x1, LOW);
    }
    if (millis() - time1 > Tx && millis() - time1 <= Tx + Tv) {
      digitalWrite(v1, HIGH);
      digitalWrite(db1, HIGH);
      digitalWrite(xb1, LOW);
      ttx1 = 0;
      ttd1 = 0;
      ttv1 = 1;
      ttxb1 = 0;
      ttdb1 = 1;
      dem1 = Tx;
      dem3 = Td;
      if (millis() - time2 >= 1000) {
        dem2 = (dem2 - 1000);
        showNumber(dem2 / 1000);
        time2 = millis();
      }
    }
    else {
      digitalWrite(v1, LOW);
    }
    if (millis() - time1 > Tx + Tv && millis() - time1 <= Td + Tx + Tv) {
      digitalWrite(d1, HIGH);
      digitalWrite(db1, LOW);
      digitalWrite(xb1, HIGH);
      ttx1 = 0;
      ttd1 = 1;
      ttv1 = 0;
      ttxb1 = 1;
      ttdb1 = 0;
      dem1 = Tx;
      dem2 = Tv;
      if (millis() - time2 >= 1000) {

        dem3 = (dem3 - 1000);
        showNumber(dem3 / 1000);
         
        time2 = millis();
      }
    }
    else {
      digitalWrite(d1, LOW);
    }


    if (millis() - time1 > Td1 + Tx1 + Tv1) {
      time1 = millis();
    }
    gui();
  }
  
}
void  gui(){
  if(ttx1 == 1){
  Blynk.virtualWrite(V1, dem1 / 1000);
  }
  else Blynk.virtualWrite(V1, 0);
  if(ttv1 == 1){
  Blynk.virtualWrite(V2, dem2 / 1000);
  }
  else Blynk.virtualWrite(V2,0);

  if(ttd1 == 1){
  Blynk.virtualWrite(V3, dem3 / 1000);
  }
  else Blynk.virtualWrite(V3, 0);
  if(ttx2 == 1){
  Blynk.virtualWrite(V4, dem11 / 1000);}
  else Blynk.virtualWrite(V4, 0);
  if(ttv2 == 1){
  Blynk.virtualWrite(V5, dem22 / 1000);
  }
  else Blynk.virtualWrite(V5, 0);
  if(ttd2 == 1){
  Blynk.virtualWrite(V6, dem33 / 1000);  
}
else Blynk.virtualWrite(V6, 0);
}
void showNumber(int num)
{
  digit2 = num % 10 ;
  digit1 = (num / 10) % 10 ;
  //Send them to 7 segment displays
  uint8_t numberToPrint[] = {digits[digit2], digits[digit1]};
  sr.setAll(numberToPrint);
}
void showNumber2(int num2)
{
  digit4 = num2 % 10 ;
  digit3 = (num2 / 10) % 10 ;
  //Send them to 7 segment displays
  uint8_t numberToPrint1[] = {digits[digit4], digits[digit3]};
  sr1.setAll(numberToPrint1);
}
