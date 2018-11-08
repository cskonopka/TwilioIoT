#include "wio_tracker.h"

#define BUTTON_PIN  (D38)

WioTracker wio = WioTracker();
int counter = 0;

void setup()
{
  // Button Setup
  pinMode(BUTTON_PIN, INPUT);

  // Wio Setup
  SerialUSB.println("Wait for power on...");
  wio.Power_On();
  SerialUSB.println("Power On O.K!");

  // Wio Initialization
  while (!wio.init()) {
    delay(1000);
    SerialUSB.println("Accessing network...");
  }
  SerialUSB.println("Initialize done...");

  // Connect to the network
  bool ret = wio.waitForNetworkRegister();
  if (true == ret) {
    SerialUSB.println("Network accessed!");
  } else {
    SerialUSB.println("Network failed!");
    return;
  }
  SerialUSB.println("Ready!");
}

void loop()
{
  // Define button state
  int buttonState = digitalRead(BUTTON_PIN);

  // Define the counter functionality
  if (buttonState == 0) {
    // Zero counter
    counter = 0;
  } else {
    // Continue counter
    counter++;
    // Catch only 1 press, no duplicate calls
    if (counter == 1) {
      SerialUSB.println("Happy Birthday sent!");
      // Personal phone number
      wio.callUp("PERSONAL_PHONE_NUMBER");
    }
  }
  delay(100);
}
