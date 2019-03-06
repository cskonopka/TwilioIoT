#include <WioLTEforArduino.h>

#define ROTARY_ANGLE_PIN  (WIOLTE_20)

WioLTE Wio;

void setup() {
  pinMode(WIOLTE_D20, OUTPUT);
}

void loop() {
  digitalWrite(WIOLTE_D20, HIGH);       // sets the digital pin 13 on
  delay(1000);                  // waits for a second
  digitalWrite(WIOLTE_D20, LOW);        // sets the digital pin 13 off
  SerialUSB.println("LOW");
  delay(1000);
}
