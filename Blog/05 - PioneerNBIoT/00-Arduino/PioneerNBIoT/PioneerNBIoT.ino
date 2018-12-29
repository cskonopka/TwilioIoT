#include <Seeed_ws2812.h>
#include <BreakoutSDK.h>

static const char *device_purpose = "Dev-Kit";
static const char *psk_key = "YOUR_PSK";
Breakout *breakout = &Breakout::getInstance();
WS2812 strip = WS2812(1, RGB_LED_PIN);

void enableLed() {
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, HIGH);
  strip.begin();
  strip.brightness = 5;
}

void setup() {
  owl_log_set_level(L_INFO);
  LOG(L_WARN, "Arduino setup() starting up\r\n");
  enableLed();
  strip.WS2812SetRGB(0, 0x20, 0x20, 0x00);
  strip.WS2812Send();
  breakout->setPurpose(device_purpose);
  breakout->setPSKKey(psk_key);
  breakout->setPollingInterval(1 * 60);  // Optional, set to 1 minute
  // Powering the modem and starting up the SDK
  LOG(L_WARN, "Powering on module and registering...");
  breakout->powerModuleOn();
  const char command[] = "Hello World from BreakoutSDK test app";
  if (breakout->sendTextCommand(command) == COMMAND_STATUS_OK) {
    LOG(L_INFO, "Tx-Command [%s]\r\n", command);
  } else {
    LOG(L_INFO, "Tx-Command ERROR\r\n");
  }
  // Set RGB-LED to green
  strip.WS2812SetRGB(0, 0x00, 0x40, 0x00);
  strip.WS2812Send();
  LOG(L_WARN, "... done powering on and registering.\r\n");
  LOG(L_WARN, "Arduino loop() starting up\r\n");
}

void your_application_example() {
  if (breakout->hasWaitingCommand()) {
    char command[141];
    size_t commandLen = 0;
    bool isBinary     = false;
    command_status_code_e code = breakout->receiveCommand(140, command, &commandLen, &isBinary);
    switch (code) {
      case COMMAND_STATUS_OK:
        LOG(L_INFO, "Rx-Command [%.*s]\r\n", commandLen, command);
        break;
      case COMMAND_STATUS_ERROR:
        LOG(L_INFO, "Rx-Command ERROR\r\n");
        break;
      case COMMAND_STATUS_BUFFER_TOO_SMALL:
        LOG(L_INFO, "Rx-Command BUFFER_TOO_SMALL\r\n");
        break;
      case COMMAND_STATUS_NO_COMMAND_WAITING:
        LOG(L_INFO, "Rx-Command NO_COMMAND_WAITING\r\n");
        break;
      default:
        LOG(L_INFO, "Rx-Command ERROR %d\r\n", code);
    }
  }
}

void loop() {
  your_application_example();
  breakout->spin();
  delay(50);
}