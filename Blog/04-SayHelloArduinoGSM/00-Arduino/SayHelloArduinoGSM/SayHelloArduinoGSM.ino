/*
 04 - Say hello to the Arduino MKR GSM 1400 with Programmable Wireless and Go
 Christopher Konopka
 Twilio IoT Developer Evangelist
 https://github.com/cskonopka/TwilioIoT/tree/master/Blog/04%20-%20SayHelloArduinoGSM
*/

// Include the GSM library
#include <MKRGSM.h>

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

void setup() {
  // Initialize serial poirt
  Serial.begin(152000);

  // Connects to the GSM network identified on the SIM card
  gsmAccess.begin();
  Serial.println("GSM initialized");
}

void loop() {
  // Send Command to your server via the short code "2936"
  // The shortcode "2936" is routed to the Commands Callback Url of your SIM
  sms.beginSMS("2936");

  // The print function is where you define the string message you would like to send
  sms.print("hello world");

  // endSMS completes the SMS command
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");

  while (1) {
    delay(4000);
  }
}
