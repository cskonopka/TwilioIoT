## 03 - Saying Happy Birthday to my Grandmother with #TwilioIoT

![](https://image.ibb.co/j5ctA0/Blog-Banner.png)

When it came to my Grandmother's 90th birthday I was stumped about what to get her as a gift. She has been a big influence in my life and I wanted to do something that showed that. I knew I wanted to integrate Twilio in some way because when she was younger she was a switchboard operator for Bell Telephone. Then it hit me like a ton of phones. I will create a hardware device with a Programmable Wireless SIM and a button. When she presses the button a voice call will be routed to her phone and an operator will tell her how much I love her and wish her a happy birthday. 

### What I needed to send some birthday love

- [Twilio SIM](https://www.seeedstudio.com/Twilio-Wireless-SIM-Card-p-3065.html)
- [Twilio Phone Number](https://www.twilio.com/console/phone-numbers/search)
- [TwiML Bins](https://www.twilio.com/console/runtime/twiml-bins)
- [Wio LTE Cat.1 board by Seeed Studio](https://www.seeedstudio.com/Wio-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html)
- Antenna
- Lithium Battery
- Micro-USB cable
- [Seeed Studio Grove Button](https://www.seeedstudio.com/Grove-Button-p-766.html)
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Wio LTE Arduino Library](https://github.com/Seeed-Studio/Wio_LTE_Arduino_Library)

### How does this work?

I want to generate a call from my [Twilio SIM](https://www.twilio.com/console/wireless/orders/new) using the [Seeed Studio Wio board](https://www.seeedstudio.com/Wio-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html). A button is connected to the Wio and is used for physical interaction. When the button is clicked it will trigger a callback in Twilio. To make the call I need to create some [TwiML](https://www.twilio.com/docs/voice/twiml) which will respond to the callback. Then when the call connects I need another piece of TwiML which will play a [Text-to-Speech](https://www.twilio.com/docs/voice/twiml/say/text-speech) message using [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech). Since we are connecting a SIM card on a board to a real device, we'll use a little workaround to get the message played. By including a `url` attribute on [`<Number>`](https://www.twilio.com/docs/voice/twiml/number) Twilio will play the TwiML to the person who answers the phone before they are connected. This is known as a [whisper](https://www.twilio.com/docs/glossary/call-whisper).

### Purchase a Twilio number & register a Twilio SIM

First things first, purchase a [Twilio phone number](https://www.twilio.com/console/phone-numbers/search). This phone number will be used to route the call from the [Programmable Wireless](https://www.twilio.com/docs/wireless) SIM to my grandmother’s phone using voice data. 

Remove the Twilio SIM from it’s packaging, [register and activate your SIM](https://www.twilio.com/docs/wireless/tutorials/how-to-order-and-register-your-first-sim). 

### Creating the Text-to-Speech TwiML with Amazon Polly 

I wanted to create a Text-to-Speech message using [TwiML](https://www.twilio.com/docs/glossary/what-is-twilio-markup-language-twiml). TwiML is the XML-based Twilio Markup Language used to help build voice and SMS applications. A [TwiML Bin](https://www.twilio.com/console/runtime/twiml-bins) is a way to prototype an interaction with TwiML without having to create and host a web server yourself. 
To do this I used the TwiML [<Say>](https://www.twilio.com/docs/voice/twiml/say) verb to create a message that will be verbalized when my grandmother answers the phone call. And to make it more realistic I used the [voice attribute](https://www.twilio.com/docs/voice/twiml/say#voice) to 
select a [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech#amazon-polly) voice so it sounded more realistic.
  
- Navigate to [Runtime](https://www.twilio.com/console/runtime/overview) in the Twilio Console
- Click TwiML Bins
- Click the + to add a new [TwiML Bin](https://www.twilio.com/console/runtime/twiml-bins)
- Create new a new TwiML <Response>
- Add the <Say> verb 
- Add the [voice attribute](https://www.twilio.com/docs/voice/twiml/say#voice) to specify an [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech#amazon-polly) voice 
- Specify the language with the [language attribute](https://www.twilio.com/docs/voice/twiml/say#attributes-language)
- Press save

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Response>
    	<Say voice="Polly.Joey" language="en-US">Happy Birthday Gram! I love you.</Say>
</Response>
```
Save the TwiML Bin and make sure to copy the URL of the TwiML Bin, it will used momentarily.

### Creating the Programmable Voice URL TwiML for your SIM

The [Text-to-Speech](https://www.twilio.com/docs/voice/twiml/say/text-speech) part is ready, next I need to route the inbound [Programmable Voice](https://www.twilio.com/docs/voice) call from the SIM using TwiML. These are the steps to set this up:

- Navigate to [Programmable Wireless](https://www.twilio.com/console/wireless) in the Twilio Console
- In [Overview](https://www.twilio.com/console/wireless), select the newly registered SIM
- Click the Programmable Voice & SMS tab
- Under Programmable Voice & SMS change the Voice Url to TwiML 
- Paste the TwiML below into the box
- Add your Twilio phone number to the [callerId attribute](https://www.twilio.com/docs/voice/twiml/dial#callerid) of the Dial Verb
- Paste the TwiML Bin URL into the [URL attribute](https://www.twilio.com/docs/voice/twiml/number#attributes-url) of the Number noun

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Response>
    <Dial callerId="YOUR_TWILIO_NUMBER">
		<Number url="TWIML_BIN_URL">{{To}}</Number>
  	</Dial>
</Response>
```

Great! The SIM is now configured to route an outgoing voice call with a Twilio number and read a custom message to my grandmother using Text-to-Speech. That was a mouthful of cake, no doubt. Next let’s make this into a physical device. 

### Hardware setup

Unbox the [Wio LTE](https://www.seeedstudio.com/Wio-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html) board and connect the board to the computer using the Micro-USB cable provided. Depending on the operating system a [USB driver](http://wiki.seeedstudio.com/Wio_LTE_Cat.1/#install-usb-driver) may be needed to connect to the Wio.

Insert the [Twilio SIM](https://www.seeedstudio.com/Twilio-Wireless-SIM-Card-p-3065.html) you registered into the Wio’s SIM slot on the back of the board. 

Within the Wio LTE box is a cellular antenna. Remove the antenna from it’s packaging and connect the antenna to the back of the Wio where it says LTE main.

Now remove the [Seeed Studio Grove Button](https://www.seeedstudio.com/Grove-Button-p-766.html) from it’s packaging and connect the button to the D38 socket of the Wio.

### Software Environment setup

Next to download [Arduino IDE](https://www.arduino.cc/en/Main/Software). This will be used to program and upload code to the Wio. After installing the Arduino IDE download the [Wio LTE Arduino Library](https://github.com/Seeed-Studio/Wio_LTE_Arduino_Library) from GitHub and follow this guide to [install the library](https://github.com/Seeed-Studio/Wio_LTE_Arduino_Library#usage). The library is a wrapper for the [STM32F4 chip](https://en.wikipedia.org/wiki/STM32#STM32_F4) that utilizes the [Arduino Core](https://arduino.stackexchange.com/questions/35048/explain-what-is-meant-by-arduino-core) and adds [Quectel EC21-A](https://www.quectel.com/UploadFile/Product/Quectel_EC21_LTE_Specification_V1.0.pdf) modem functionality. 

Next put the Wio into Bootloader mode. This mode needs to be enabled for uploading code to the board. To enable Bootloader mode:

1. Press and hold the BOOT0 button underneath the board

IMAGE

2. Press the RST on the top of the board

IMAGE

3. Release the BOOT0 button to enable Bootloader mode

### Creating the code

https://gph.is/YeaoHh

Create a new Arduino sketch (File -> New).

Start by adding the Wio library file wio_tracker.h and defining a global variable for the [Seeed Studio Grove Button](https://www.seeedstudio.com/Grove-Button-p-766.html) above the [setup()](https://www.arduino.cc/en/Reference/Setup/) function. Next create an instance of the WioTracker using the variable wio and create a counter variable. The counter will be used to [debounce](https://www.arduino.cc/en/Tutorial/Debounce) the button so we have more control over the button’s interaction.

```arduino
#include "wio_tracker.h"

#define BUTTON_PIN  (D38)

WioTracker wio = WioTracker();
int counter = 0;
```

The [setup()](https://www.arduino.cc/en/Reference/Setup/) function  is used to initialize various aspects of the program at startup. The [pinMode()](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/) is used to set the physical pin the button is connected to and the type of functionality the board expects. Following the button setup the Wio and the modem are powered up. Then the board runs through it’s initialization process and connects the Twilio SIM to the cellular network.

```arduino
void setup()
{
  // Button Setup
  pinMode(BUTTON_PIN, INPUT);

  // Wio Power Up
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
```

The button logic for the program is created in the [loop()](https://www.arduino.cc/en/Reference/Loop?setlang=it) function. This function continually listens for changes in the state of the board. In this case it is waiting for the [button state to change](https://www.arduino.cc/en/Tutorial/StateChangeDetection). The variable buttonState reads incoming button state changes from the physical pin on the board using [digitalRead()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/).

```arduino
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
      wio.callUp("GRANDMOTHER_PHONE_NUMBER");
    }
  }
  // Restart
  delay(100);
}

```

The main if-statement manages the incoming state changes. The counter is used as a way to [debounce](https://www.arduino.cc/en/Tutorial/Debounce) the button so only a single call is made. If there was no debouncing logic the program would continue to make calls every 100ms while the button is held down.

When the button is not being pressed, state “0”, the counter will reset to 0. 

```arduino
  if (buttonState == 0) {
    // Zero counter
    counter = 0;
  }
```

When the button is pressed, state “1”, the counter starts to iterate. A nested if-statement is used to catch a specific number created by the counter. When the number 1 is caught the call is made to my grandmother using `wio.callUp("GRANDMOTHER_PHONE_NUMBER")`.  

```arduino
 else {
    // Continue counter
    counter++;
    // Catch only 1 press, no duplicate calls
    if (counter == 1) {
      SerialUSB.println("Happy Birthday sent!");
      wio.callUp("GRANDMOTHER_PHONE_NUMBER");
    }
  }

```

That's all the code we need, to transfer this to the Wio board press upload. 

When the upload finishes disconnect the Wio from the Micro-USB cable. Connect the lithium battery to the board. 

### Box it up

For the party I placed the board, battery and antenna in a small box to make it more presentable. I tried my best to make it fancy but I felt it looked cooler exposed with a few parts hidden. [During the event I sat down with my grandmother and presented the box to her](https://www.youtube.com/watch?v=7xkZ7l0JMBI). I managed to record a short video of her demoing it for the first time and she was jazzed about it. I can’t imagine what it must be like seeing the evolution of communication from her eyes and ears. If you are curious what happened I recorded the video so people can see her reaction in real time.

### Thoughts

After creating this project I realized this could be useful for individuals who may be separated by long distances. And instead of Text-to-Speech an audio file could be used to send personal messages like voicemail in reverse. Sending a love note and reminding others you care is important. What type of #TwilioIoT projects are you building with [Programmable Wireless](https://www.twilio.com/docs/wireless)? Reach out on [Twitter](http://twitter.com/cskonopka) with your comments, questions and projects anytime. See you on the web, ride the wave.
