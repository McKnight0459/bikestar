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


#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

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
//Fona Init
 
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  // make it slow so its easy to read!
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }
  
  Serial.println("FONA Ready");
}

  
char fonaInBuffer[64];          //for notifications from the FONA

void loop() {
  
  char* bufPtr = fonaInBuffer;    //handy buffer pointer
  
  if (fona.available())      //any data available from the FONA?
  {
    uint8_t slot = 0;            //this will be the slot number of the SMS
    int charCount = 0;
    //Read the notification into fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaInBuffer)-1)));
    
    //Add a terminal NULL to the notification string
    *bufPtr = 0;
    
    //Scan the notification string for an SMS received notification.
    //  If it's an SMS message, we'll get the slot number in 'slot'
    if (1 == sscanf(fonaInBuffer, "+CMTI: \"SM\",%d", &slot)) {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[32];  //we'll store the SMS sender number in here
      
      // Retrieve SMS sender address/phone number.
      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("Didn't find SMS message in slot!");
      }
      Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);

      // Retrieve SMS value.
      uint16_t smslen;
      if (! fona.readSMS(slot, replybuffer, 250, &smslen)) { // pass in buffer and max len!
        Serial.println("Failed!");
      }
      Serial.print(F("***** SMS #")); 
      Serial.print(slot); 
      Serial.print(" ("); 
      Serial.print(smslen); 
      Serial.println(F(") bytes *****"));
      Serial.println(replybuffer);
      Serial.println(F("*****"));
  
       // now, should this SMS ring the doorbell??
      // we use the C function `strstr` because we're dealing with a character array,
      // not the typical arduino "string" object
      if (strstr(replybuffer, "Quiet\0") != NULL)
      {
        QuietMode();
      }else if (strstr(replybuffer, "Loud\0") != NULL)
      {
        LoudMode();
      }
      
      // delete an SMS
              Serial.print(F("Delete #"));
        uint8_t smsn = slot;

        Serial.print(F("\n\rDeleting SMS #")); Serial.println(smsn);
        if (fona.deleteSMS(smsn)) {
          Serial.println(F("OK!"));
        } else {
          Serial.println(F("Couldn't delete"));
        }
    }
  }
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

