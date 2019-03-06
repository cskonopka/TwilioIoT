#include <WioLTEforArduino.h>

#define ROTARY_ANGLE_PIN  (WIOLTE_A4)
#define INTERVAL          (500)

WioLTE Wio;

void setup() {
  delay(200);

  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");

  SerialUSB.println("### I/O Initialize.");
  Wio.Init();

  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyGrove(true);
  delay(500);

  SerialUSB.println("### Setup pin mode.");
  pinMode(ROTARY_ANGLE_PIN, INPUT_ANALOG);
}

void loop() {
  int rotaryAngle = analogRead(ROTARY_ANGLE_PIN);
  SerialUSB.println(rotaryAngle);  
  delay(INTERVAL);
}
