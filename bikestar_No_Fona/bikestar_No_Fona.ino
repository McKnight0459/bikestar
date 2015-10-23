/***************************************************
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
THIS CODE IS STILL IN PROGRESS!

Open up the serial console on the Arduino at 115200 baud to interact with FONA


This code will receive an SMS, identify the sender's phone number, and automatically send a response

*/
#define LED_1 5
#define LED_2 6
#define LED_3 7
#define LED_4 8
#define LED_5 9
#define BUZZER 10


void setup() {

 /*****************************************************************/
 //Pin Init
  pinMode(BUZZER, OUTPUT);
    pinMode(LED_1, OUTPUT);
      pinMode(LED_2, OUTPUT);
        pinMode(LED_3, OUTPUT);
          pinMode(LED_4, OUTPUT);
            pinMode(LED_5, OUTPUT);
  digitalWrite(BUZZER,LOW);     //Buzzer不响
   digitalWrite(LED_1,LOW);      //1号LED熄灭
    digitalWrite(LED_2,LOW);      //2号LED熄灭
      digitalWrite(LED_3,LOW);      //3号LED熄灭
        digitalWrite(LED_4,LOW);      //4号LED熄灭
          digitalWrite(LED_5,LOW);      //5号LED熄灭
 /*****************************************************************/
   while (!Serial);
  Serial.begin(115200);
}
void loop() {

        QuietMode();
        delay(5000);
        LoudMode();

}

void QuietMode()
{
      Serial.println("QuietMode!\n");
  LED_on();
  delay(5000);
  LED_off();
}

void LoudMode()
{
    Serial.println("LoudMode!\n");
  LED_on();
  Buzzer_on();
  delay(5000);
  LED_off();
  Buzzer_off();
}
void LED_on()
{
  Serial.println("LED On!\n");
   digitalWrite(LED_1,HIGH);      //1号LED熄灭
    digitalWrite(LED_2,HIGH);      //2号LED熄灭
      digitalWrite(LED_3,HIGH);      //3号LED熄灭
        digitalWrite(LED_4,HIGH);      //4号LED熄灭
          digitalWrite(LED_5,HIGH);      //5号LED熄灭
}
void  LED_off()
{
    Serial.println("LED OFF!\n");
  digitalWrite(LED_1,LOW);      //1号LED熄灭
    digitalWrite(LED_2,LOW);      //2号LED熄灭
      digitalWrite(LED_3,LOW);      //3号LED熄灭
        digitalWrite(LED_4,LOW);      //4号LED熄灭
          digitalWrite(LED_5,LOW);      //5号LED熄灭

}
void Buzzer_on()
{
  Serial.println("Buzzer On!\n");
  digitalWrite(BUZZER,HIGH);      //BUZZER ON..
}
void  Buzzer_off()
{
    Serial.println("Buzzer OFF!\n");
  digitalWrite(BUZZER,LOW);      //BUZZER OFF.

}

