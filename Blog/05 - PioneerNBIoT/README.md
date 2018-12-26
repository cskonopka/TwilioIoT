07 - Draft - Pioneer NB-IoT with Twilio's Alfa Development Kit

![](null)

*[NB-IoT](https://www.twilio.com/wireless/narrowband), *also known as *Narrowband-IoT*, is a new cellular technology that promises low cost, low power consumption, wide area coverage and long battery life. These characteristics help make "[smart devices](https://en.wikipedia.org/wiki/Smart_device)" a reality.

[T-Mobile](https://iot.t-mobile.com/narrowband/) has deployed [NB-IoT coverage](https://www.gsma.com/iot/deployment-map/) in the United States and Twilio is the first company to provide an NB-IoT developer kit. [Twilio's Alfa Developer Kit](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit) features the Alfa development board created in collaboration with [SeeedStudio](https://www.seeedstudio.com/). The Alfa development board can access the T-Mobile NB-IoT network using the new [Twilio Narrowband SIM](https://www.twilio.com/wireless/narrowband). Once on the network, developers can exchange data between multiple NB-IoT kits using the Twilio Breakout SDK. 

 

This post demonstrates how to connect to T-Mobile's NB-IoT network using Twilio's Alfa Developer Kit. Once connected, we'll send a "hello world" message over the network using the Breakout SDK. You can also find the the completed project on [GitHub](https://github.com/cskonopka/TwilioIoT) under [TwilioIoT](https://github.com/cskonopka/TwilioIoT). 

Ready to say "hello?".  Let's connect!

[https://media.giphy.com/media/IThjAlJnD9WNO/giphy.gif]

## Prerequisites to Connecting to Narrowband

Before you begin, you'll need to either create a new Twilio account or log in to an existing account. You can [sign up for a new account for free](https://twilio.com/try-twilio).

Beyond a Twilio account, here is all of the hardware and software you'll need to put in place to get connected.

### Hardware Requirements

> * [Twilio Alfa Development Kit](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit)

> * [Twilio Narrowband SIM](https://www.twilio.com/wireless/narrowband)

> * Antenna

> * Micro USB cable

> * Lithium battery

### Software Requirements

> * [Arduino IDE](https://www.arduino.cc/en/Main/Software)

> * [Breakout SDK](https://github.com/twilio/Breakout_Arduino_Library)

> * cURL

## Explore the Alfa Development Kit

The Alf Development Kit ships with the Alfa development board that is specifically designed for connecting to T-Mobile's NB-IoT network. Also included are several hardware attachments by SeeedStudio that can be used to develop a NB-IoT "smart device".

Open the Alfa Development Kit box. The kit contains: 

> * Programmable Wireless Narrowband IoT SIM (full size, mini, micro, and nano) 

> * Alfa Development board 

> * LTE antenna 

> * GPS antenna 

> * Set of Grove sensors

> <sub>    * Pushbutton

> <sub>    * Ultrasonic

> <sub>    * Temperature/Humidity

> * Lithium battery 

> * Micro-USB cable 

> * Additional cabling

![](null)

## Set up the Twilio Narrowband SIM

![](null)

Remove the Twilio Narrowband SIM from the Alfa Development Kit. Next, [register and activate your Narrowband SIM](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit#step-1-register-your-narrowband-sim) in the Twilio Console. The process for the Narrowband SIM follows the same procedure as the [Twilio Programmable Wireless SIM](https://www.twilio.com/docs/wireless/tutorials/how-to-order-and-register-your-first-sim).

## Connecting the pieces

Break out the Nano SIM (smallest size) from the Twilio SIM card. 

![](null)

Remove the Alfa development board from the the Alfa Developer Kit.

![](null)

Insert the Twilio Narrowband SIM into the SIM slot underneath the board.

![](null)

Next, attach the LTE antenna to the board.

![](null)

Attach the battery lithium battery. The lithium battery is recommended to be plugged in at all times since the USB power source does not provide sufficient power for the board at peak levels.

![](null)

Connect the Alfa development board to the computer using the Micro-USB cable provided. You are geared up to connect to the network. 

## Configure the NB-IoT Kit Firmware

Before we can start programming the board we need to update the board's firmware. To do this on a Macintosh we will need [Homebrew](https://brew.sh/) to install [dfu-util](http://dfu-util.sourceforge.net/). Instructions for installing dfu-util for [Windows](https://github.com/redbear/Duo/blob/master/docs/dfu-util_installation_guide.md#windows) and [Linux](https://github.com/redbear/Duo/blob/master/docs/dfu-util_installation_guide.md#linux) can be found [here](https://github.com/redbear/Duo/blob/master/docs/dfu-util_installation_guide.md).

If you don't yet have it installed, open a terminal and paste the following to install Homebrew.

``` bash

/usr/bin/ruby -e "$(curl -fsSL [https](https://raw.githubusercontent.com/Homebrew/install/master/install)[://raw.githubusercontent.com/Homebrew/install/master/install](https://raw.githubusercontent.com/Homebrew/install/master/install))

```

Once the installation is complete install the [dfu-util](http://dfu-util.sourceforge.net/) package. This package is used to download and upload firmware to and from USB connected devices. dfu-util 0.9 or greater is preferred if available.

``` bash

brew install dfu-util libusb

```

For Windows users there is a different set of [USB Drivers](http://wiki.seeedstudio.com/Wio_LTE_Cat_M1_NB-IoT_Tracker/#install-usb-driver) that are needed.

## Set up the software environment

The Alfa development board uses the [Arduino IDE](https://www.arduino.cc/en/main/software) to program the microcontroller. Twilio has developed a NB-IoT specific software development kit called the [Breakout SDK](https://github.com/twilio/Breakout_Arduino_Library#setting-up-your-development-environment). This SDK makes it possible for devices to send M2M Commands over the T-Mobile NB-IoT network. The Breakout SDK can be found on [GitHub](https://github.com/twilio/Breakout_Arduino_Library#setting-up-your-development-environment).

![](null)

Download the *Breakout_Arduino_Library.zip* from GitHub. Open the Arduino IDE and add the .zip to the Arduino IDE Library. 

Go to *Sketch > Include Library > Add .ZIP library* and select the *Breakout_Arduino_Library.zip*. 

![](null)

After the .zip file has been installed we need to install a set of board cores. The Alfa development board is based on the [STM32F4](https://en.wikipedia.org/wiki/STM32) chipset. To develop on the board we need to download the STM32F4 cores in the Arduino IDE.

Go to Arduino > Preferences. Copy the following URL into the Additional Boards Manager URLs field:

*[https://raw.githubusercontent.com/Seeed-Studio/Seeed_Platform/master/package_seeeduino_boards_index.json](https://raw.githubusercontent.com/Seeed-Studio/Seeed_Platform/master/package_seeeduino_boards_index.json)*

![](null)

Click OK. 

The STM32F4 boards will now be available in the Arduino IDE *Boards Manager. *Next open the *Boards Manager* to install the STM32F4 board cores.

In the *Boards Manager* search for "Seeed". Find and select the "Seeed STM32F4 Boards" version "1.2.3+" and click install.

![](null)

Restart the Arduino IDE.

With the STM32F4 cores installed the Alfa development board is now ready to be programmed. Next select the board and the board port.

> * Click Tools > Boards > Wio Tracker LTE

![](null)

> * Click Tools > Port > {Your Modem Port Here}

> <sub>    * OSX: /dev/{cu|tty}.usbmodem{XXXX}

> <sub>    * Linux: /dev/ttyACM{X}

> <sub>    * Windows: COM{X}

![](null)

## Configure the HelloWorld.ino file

Open the Hello World example provided by the Breakout SDK.


In the HelloWorld.ino we need to make a few modifications so we can connect to the T-Mobile NB-IoT network.

Find the *psk_key* in the "HelloWorld.ino" file.

![](null)

Each Alfa development board has a unique SIM ICCID and Pre-Shared Key (*psk*). The psk for the board we are using needs to be copied into the HelloWorld.ino sketch. This key is required to connect to the T-Mobile Narrowband network.

> * Navigate to Programmable Wireless in the Twilio Console

> * Click SIMs

> * Find the Narrowband SIM that was previously registered

> * Under the tab *Breakout SDK* find Credentials

> * Where it says Pre-Shared Key (psk) click the eye logo to reveal the key

> * Copy the psk

> * Paste your psk into the HelloWorld.ino file in the code above

![](null)

After the psk is set let's change the *[setPollingInterval](https://www.twilio.com/docs/wireless/nb#sms-commands-versus-ip-based-polling-commands). *This determines how often the Breakout SDK will [poll](https://www.twilio.com/docs/wireless/nb#sms-commands-versus-ip-based-polling-commands) for a new Command. Find the line *breakout->setPollingInterval. *

![](null)

Change the 'setPollingInterval' interval from *10* to *1. *This will change the [polling](https://www.twilio.com/docs/wireless/nb#what-is-polling) time from 10 minutes to 1 minute.

```arduino

  breakout->setPollingInterval(1 * 60);

```

Below is the complete Arduino sketch. Further details on how to the Breakout SDK can found on [GitHub](https://github.com/twilio/Breakout_Arduino_Library).

``` arduino

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

```

## Enter Bootloader Mode

To upload code to the development board the unit needs to be put into Bootloader mode. 

> <sub>    1. Press and hold the BOOT0 button underneath the Developer Board

> ![](null)

> <sub>    2. Press and hold the RST on the top of the Developer Board

		![](null)

> <sub>    3. Release the RST on the top of the Developer Board

> <sub>    4. Release the BOOT0 button to enable Bootloader mode

> ![](null)

Press Upload in the Arduino IDE. 

![](null)

![](null)

After the code has been uploaded to the Alfa development board press the RST button. This will take the board out of Bootloader mode. The completed code can be found on the [TwilioIoT GitHub](https://github.com/cskonopka/TwilioIoT/tree/master/Blog).

## Connect to the network and sending a Command

After resetting the board start the NB-IoT network registration process. This will register the board on the network and allocate bandwidth for the device. During this process the Network Connectivity LED will glow orange. 

![](null)

Open the Serial Monitor to observe the board registering and connecting to the network. 

![](null)

When the Alfa development board successfully registers to the NB-IoT network the Network Connectivity LED will glow blue. The following message will display in the Arduino Serial Monitor when the connection is stable:

![](null)

When the board successfully connects to the NB-IoT network, the Breakout SDK will be initialized. 

![](null)

This is the Serial Monitor output when the Breakout SDK sends a Command to Twilio.

![](null)

Every Command sent and received by the Breakout SDK is logged. Commands sent over the NB-IoT network can be found in the Twilio Console under Programmable Wireless.

> * Navigate to Programmable Wireless in the Twilio Console

> * Click SIMs

> * Find the Narrowband SIM that was previously registered

> * Click the *Commands* tab

![](null)

## Receive a Command with the Breakout SDK

The Breakout SDK will poll for a new Command every minute. Using [cURL](https://en.wikipedia.org/wiki/CURL), you can send a Command to the NB-IoT board by using the Sim unique name. 

**``` **bash

curl -X POST https://wireless.twilio.com/v1/Commands \ --data-urlencode "Sim=Breakout" \ --data-urlencode "Command=POST to Breakout SDK" \ -u ACXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX:your_auth_token

```

Watch the Arduino IDE Serial Monitor to see the Command as it is received:

**
**![](null)

## How does it feel to be one of the first pioneers of NB-IoT?

[[https://media.giphy.com/media/3oKIPz1hL5ErsG8P6w/giphy.mp4](https://media.giphy.com/media/3oKIPz1hL5ErsG8P6w/giphy.mp4)]
[https://media.giphy.com/media/1xRPLACcmDNNS/giphy.mp4](https://media.giphy.com/media/1xRPLACcmDNNS/giphy.mp4)

The future of "things" using NB-IoT
It's a special time for IoT development on both the hardware and software side. Many processes are becoming optimized... and many are still shrouded in mystery. 

These "things" impact our daily lives – often without us even realizing it. Every segment from scooters to home automation now has some element of interconnectivity. 

NB-IoT takes it a step further still. The low cost, low power consumption, wide area coverage and long battery life of Narrowband make "Smart Devices" even smarter. Become an IoT pioneer with Twilio Narrowband. Let's build dreams together.

## *If you ever want to chat about IoT, hardware or modular synthesizers ping me anytime on Twitter or via Email. Let's connect.*

> * Email: ckonopka@twilio.com

> * Github:[ cskonopka](http://github.com/cskonopka)

> * Twitter:[ @cskonopka](http://twitter.com/cskonopka)

