#include <WioLTEforArduino.h>
#include <stdio.h>

#define WEBHOOK_EVENTNAME "EVENT-NAME"
#define WEBHOOK_KEY       "KEY"
#define WEBHOOK_URL       "https://maker.ifttt.com/trigger/"WEBHOOK_EVENTNAME"/with/key/"WEBHOOK_KEY
#define INTERVAL          (60000)

WioLTE Wio;

void setup() {
  delay(200);

  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");
  SerialUSB.println("### I/O Initialize.");
  Wio.Init();
  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyLTE(true);
  delay(500);
  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  SerialUSB.println("### Setup completed.");
  delay(1000);
}

void loop() {
  char data[100];
  int status;

  SerialUSB.println("### Post.");
  sprintf(data, "{\"value1\":\"%lu\", \"value2\":\"%lu\", \"value3\":\"%lu\"}", millis() / 1000, millis() / 100, millis() / 10);

  SerialUSB.print("Post:");
  SerialUSB.print(data);
  SerialUSB.println("");
  SerialUSB.println(WEBHOOK_URL);
  SerialUSB.println("");
  if (!Wio.HttpPost(WEBHOOK_URL, data, &status)) {
    SerialUSB.println("### ERROR! ###");
    goto err;
  }
  SerialUSB.print("Status:");
  SerialUSB.println(status);

err:
  SerialUSB.println("### Wait.");
  delay(INTERVAL);
}
