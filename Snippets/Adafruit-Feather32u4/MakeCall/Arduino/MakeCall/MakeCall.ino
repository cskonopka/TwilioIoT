#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

#define FONA_RX  9
#define FONA_TX  8
#define FONA_RST 4
#define FONA_RI  7

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

char replybuffer[255];

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
  fonaSerial->print("AT+CNMI=2,1\r\n");
}

void loop() {
  uint16_t smslen;
  int index = 1;
  if (!fona.callPhone("NUMBER_YOU_WISH_TO_CALL")) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }
  delay(100000);
}
