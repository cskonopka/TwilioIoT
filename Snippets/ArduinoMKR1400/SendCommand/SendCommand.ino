// Include the GSM library
#include <MKRGSM.h>

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];

void setup() {
  // Initialize serial poirt
  Serial.begin(9600);

  // connection state
  boolean connected = false;

  // Connects to the GSM network identified on the SIM card
  gsmAccess.begin();
  Serial.println("GSM initialized");

  // Send Command to your server via the short code "2936"
  // The shortcode "2936" is routed to the Commands Callback Url of your SIM
  sms.beginSMS("2936");

  // The print function is where you define the string message you would like to send
  sms.print("Hello World from the MKR 1400");

  // endSMS completes the SMS command
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}

void loop() {
  int c;

  // If there are any SMSs available()
  if (sms.available()) {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#') {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them
    while ((c = sms.read()) != -1) {
      Serial.print((char)c);
    }

    Serial.println("\nEND OF MESSAGE");

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }

  delay(1000);

}
