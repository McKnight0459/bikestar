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
#define LED_R 5
#define LED_G 6
#define LED_B 7

#define TX_PIN 9
#define BIT_PERIOD 700
#define TIMES 22
 
int PWMValue = 0;

float times[TIMES] = {
  0, .0015, .003, .0045, .0092, .0122, .0161, .0214, .0268, .0298, .0352, .0413, .0436, .0505, .0535, .0574, .062, .0673, .0719, .0757, .0803 };


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
 // pinMode(TX_PIN, OUTPUT);
  void RGB_init(); 
  
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
        ring_bell();
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

void ring_bell()
{
  Serial.println("Flashing!\n");
  RGB_run();
  delay(5000);
  RGB_off();
}
void  RGB_off()
{
  analogWrite(LED_R,0);      //高电平255 = 占空比是100%，IO相当于输出高电平，红色LED熄灭
  analogWrite(LED_G,0);     //高电平255 = 占空比是100%，IO相当于输出高电平，绿色LED熄灭
  analogWrite(LED_B,0);     //高电平255 = 占空比是100%，IO相当于输出高电平，蓝色LED熄灭
}
void RGB_init()
{   

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  analogWrite(LED_R,255);      //高电平255 = 占空比是100%，IO相当于输出高电平，红色LED熄灭
  analogWrite(LED_G,255);     //高电平255 = 占空比是100%，IO相当于输出高电平，绿色LED熄灭
  analogWrite(LED_B,255);     //高电平255 = 占空比是100%，IO相当于输出高电平，蓝色LED熄灭
}

void RGB_run()
{
  int i = 0;
  PWMValue = 255;
  for(i = 0 ; i < 255 ; i++)    //红色逐渐变亮
  {
    analogWrite(LED_R,PWMValue--);
    analogWrite(LED_G,255);
    analogWrite(LED_B,255);
    delay(10);                  
  }
  PWMValue = 0;
  for(i = 0 ; i < 255 ; i++)    //逐渐变暗
  {
    analogWrite(LED_R,PWMValue++);
    analogWrite(LED_G,255);
    analogWrite(LED_B,255);
    delay(10);
  }
  
   PWMValue = 255;
  for(i = 0 ; i < 255 ; i++)    //绿色逐渐变亮
  {
    analogWrite(LED_R,255);
    analogWrite(LED_G,PWMValue--);
    analogWrite(LED_B,255);
    delay(10);
  }
  PWMValue = 0;
  for(i = 0 ; i < 255 ; i++)    //逐渐变暗
  {
    analogWrite(LED_R,255);
    analogWrite(LED_G,PWMValue++);
    analogWrite(LED_B,255);
    delay(10);
  }
  
  PWMValue = 255;
  for(i = 0 ; i < 255 ; i++)    //蓝色逐渐变亮
  {
    analogWrite(LED_R,255);
    analogWrite(LED_G,255);
    analogWrite(LED_B,PWMValue--);
    delay(10);
  }
  PWMValue = 0;
  for(i = 0 ; i < 255 ; i++)    //逐渐变暗
  {
    analogWrite(LED_R,255);
    analogWrite(LED_G,255);
    analogWrite(LED_B,PWMValue++);
    delay(10);
  }
  
}


