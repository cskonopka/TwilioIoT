/*
  Twilio-feather32u4-ReceiveSMSWord.ino
  Christopher Konopka [IoT Developer Evangelist @ Twilio]
*/

#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7

char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t type;
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

char fonaNotificationBuffer[64];
char smsBuffer[250];

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); 
    Serial.println(imei);
  }

  fonaSerial->print("AT+CNMI=2,1\r\n");
  Serial.println("FONA Ready");
}

void loop() {
  int8_t smsnum = fona.getNumSMS();
  uint16_t smslen;
  int8_t smsn;
  int slot = 0;

  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default:
      Serial.println(F("???")); break;
  }

  if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
    smsn = 0;
    smsnum--;
  } else {
    smsn = 1;
  }

  for ( ; smsn <= smsnum; smsn++) {
    Serial.print(F("\n\rReading SMS #"));
    if (!fona.readSMS(smsn, replybuffer, 250, &smslen)) {
      Serial.println(F("Failed!"));
      break;
    }
    if (smslen == 0) {
      Serial.println(F("[empty slot]"));
      smsnum++;
      continue;
    }

    if (strcmp(replybuffer, "red") == 0) {
      Serial.println("red is the theme");
    } else {
      Serial.println("Wrong");
    }

    if (fona.deleteSMS(smsnum++)) {
      Serial.println(F("OK!"));
    } else {
      Serial.print(F("Couldn't delete SMS in slot ")); Serial.println(slot);
      fona.print(F("AT+CMGD=?\r\n"));
    }
  }
  delay(5000);
}
