
#include <DS1302.h>
#include <DHT11.h>

#define DHTPIN 2
DHT11 dht11(DHTPIN);

#define POT_PIN A0
#define RST_PIN 13
#define IO_PIN 10
#define SCK_PIN 9
DS1302 rtc(RST_PIN, IO_PIN, SCK_PIN);


float temp, humid;
int incomingByte=0;

const int E1 = 3;
const int E2 = 11;
const int E3 = 5;
const int E4 = 6;

const int M1 = 4;
const int M2 = 12;
const int M3 = 8;
const int M4 = 7;

const int LED_PIN = 10;
int motor_speed = 0;

void setup()
{
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(E4, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  rtc.halt(false);
  rtc.writeProtect(false);

  Serial.begin(9600);
  //rtc.setDOW(FRIDAY);
  //rtc.setTime(23,40,00);
  //rtc.setDate(03,11,2017);
  
}

void loop()
{
  int err;
  
  int speed_table[3][8]={{0,10,15,20,25,30,40,100},{0,20,25,30,35,40,45,100},{0,10,15,20,25,30,35,100}};
  
  float temp, humid;
  if((err=dht11.read(humid, temp))==0)
  {
    float light = analogRead(POT_PIN);
    float lux = 1023 - light;
    int len= 0;
    int count;
    char cTemps[100];
    char cTemp;
    int index =0;
    for(index=0; index<8; index++)
    {
      while(Serial.available()>0 && len <100)
      {
        cTemps[len]=Serial.read();
        len=len+1;
      }
      cTemp = cTemps[len-1];
      switch(cTemp){
        case '0':
          count=0; break;
        case '1':
          count =1; break;
        case '2':
          count =2; break;
      }
   
      motor_speed = speed_table[count][index];
      M1_advance(motor_speed);
      M2_advance(motor_speed);
      M3_advance(motor_speed);
      M4_advance(motor_speed);
      
      DisplayDHTvalue(temp, humid, lux, motor_speed);
      DisplayCurrentTime();
      delay(60000);
    }
  }
}
void M1_advance(int Speed)
{
  digitalWrite(M1, LOW);
  analogWrite(E1, Speed);
}
void M2_advance(int Speed)
{
  digitalWrite(M2, HIGH);
  analogWrite(E2, Speed);
}
void M3_advance(int Speed)
{
  digitalWrite(M3, LOW);
  analogWrite(E3, Speed);
}
void M4_advance(int Speed)
{
  digitalWrite(M4, HIGH);
  analogWrite(E4, Speed);
}


void DisplayDHTvalue(float temp, float humid, float lux, int motor_speed)
{
  Serial.print(temp);
  Serial.print(",");
  Serial.print(humid);
  Serial.print(",");
  Serial.print(lux);
  Serial.print(",");
  Serial.print(motor_speed);
  Serial.print(",");
}

void DisplayCurrentTime()
{
  Serial.print(rtc.getDateStr());
  Serial.print(",");
  Serial.println(rtc.getTimeStr());
}

void flash()
{
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

