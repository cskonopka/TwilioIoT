#include <WioLTEforArduino.h>

#define ROTARY_ANGLE_PIN  (WIOLTE_D20)
#define ROTARY_ANGLE_PIN2  (WIOLTE_D19)

WioLTE Wio;

void setup() {
  pinMode(ROTARY_ANGLE_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ANGLE_PIN2, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(ROTARY_ANGLE_PIN);
  int buttonState2 = digitalRead(ROTARY_ANGLE_PIN2);

  SerialUSB.print(buttonState);
  SerialUSB.print(" + " );
  SerialUSB.println(buttonState2);
  delay(1000);
}
