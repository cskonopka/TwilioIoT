// Wio LTE Cat.1 library
#include <WioLTEforArduino.h>

// Create a Wio instance
WioLTE Wio;

void setup() {
  // Initialization Routine
  delay(500);
  SerialUSB.println("Start WioLTE Cat.1");
  delay(500);
  SerialUSB.println("Initializing");
  Wio.Init();
  SerialUSB.println("Power ON.");
  Wio.PowerSupplyLTE(true);
  delay(500);
  SerialUSB.println("Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("ERROR: Unable to turn on");
    return;
  }
  delay(500);
  SerialUSB.println("Setup completed.");
}

void loop() {
  int r, g, b;

  // When input is received
  while (true) {
    char str[100];
    int strLen = Wio.ReceiveSMS(str, sizeof (str));

    // If the SMS is less than zero
    if (strLen < 0) {
      SerialUSB.println("ERROR: Unable to receive SMS");
      goto err;
    }

    // If the SMS is empty break and continue
    if (strLen == 0) break;
    SerialUSB.println("");

    // Look for specific color SMS from a user
    // Wio.LedSetRGB(255, 0, 0) sets the color of the RGB on the Wio LTE Cat.1
    if (strcmp (str, "red") == 0) {
      SerialUSB.println("red");
      Wio.LedSetRGB(255, 0, 0);
    }
    if (strcmp (str, "green") == 0) {
      SerialUSB.println("green");
      Wio.LedSetRGB(0, 255, 0);
    }
    if (strcmp (str, "blue") == 0) {
      SerialUSB.println(str);
      Wio.LedSetRGB(0, 0, 255);
    } else {
  
    }

    if (!Wio.DeleteReceivedSMS()) {
      SerialUSB.println("ERROR: No SMS detected");
      goto err;
    }
  }

err:
  // Restart the process in 1000ms
  delay(1000);
}
