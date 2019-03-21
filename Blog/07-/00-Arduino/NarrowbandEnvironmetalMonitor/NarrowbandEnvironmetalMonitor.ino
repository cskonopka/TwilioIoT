#include <Seeed_ws2812.h>
#include <BreakoutSDK.h>
#include <stdio.h>

#include <DHT.h>
#define SENSOR_PIN (D38)
#define DHTTYPE DHT11
DHT dht(SENSOR_PIN, DHTTYPE);

#define SEND_INTERVAL (1 * 60 * 1000)
static const char *device_purpose = "Dev-Kit";
static const char *psk_key = "PSK_KEY";
Breakout *breakout = &Breakout::getInstance();

WS2812 strip = WS2812(1, RGB_LED_PIN);

void enableLed() {
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, HIGH);
  strip.begin();
  strip.brightness = 5;
}

void setup() {
  dht.begin();
  owl_log_set_level(L_INFO);
  LOG(L_WARN, "Arduino setup() starting up\r\n");

  enableLed();
  strip.WS2812SetRGB(0, 0x20, 0x20, 0x00);
  strip.WS2812Send();

  breakout->setPurpose(device_purpose);
  breakout->setPSKKey(psk_key);
  breakout->setPollingInterval(1 * 60);  // SET TO 1 MINUTE POLLING

  LOG(L_WARN, "Powering on module and registering...");
  breakout->powerModuleOn();

  const char command[] = "Hello World from BreakoutSDK test app";

  if (breakout->sendTextCommand(command) == COMMAND_STATUS_OK) {
    LOG(L_INFO, "Tx-Command [%s]\r\n", command);
  } else {
    LOG(L_INFO, "Tx-Command ERROR\r\n");
  }

  strip.WS2812SetRGB(0, 0x00, 0x40, 0x00);
  strip.WS2812Send();
  LOG(L_WARN, "... done powering on and registering.\r\n");
  LOG(L_WARN, "Arduino loop() starting up\r\n");
}

void your_application_example() {
  char command[141];
  size_t commandLen = 0;
  bool isBinary     = false;
  // Read a command

  static unsigned long last_send = 0;

  if ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL)) {
    last_send = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    LOG(L_INFO, "The relative humidity is [%f]\r\n", humidity);
    LOG(L_INFO, "The current temperature is [%f] degrees celcius\r\n", temperature);
    char commandText[512];
    snprintf(commandText, 512, "The relative humidity is %4.2f and the current temperature is %4.2f degrees celsius", humidity, temperature);
    breakout->sendTextCommand(commandText);
  }
  delay(60000);
}

void loop() {
  // Add here the code for your application, but don't block
  your_application_example();

  // The Breakout SDK checking things and doing the work
  breakout->spin();

  delay(50);
}
