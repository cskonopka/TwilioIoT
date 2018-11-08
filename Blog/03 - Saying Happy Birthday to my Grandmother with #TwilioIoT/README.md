## 03 - Saying Happy Birthday to my Grandma with #TwilioIoT

<p align="center">
  <img width="70%" height="70%" src="https://image.ibb.co/j5ctA0/Blog-Banner.png"/>
</p>

When it came to my Grandma's 90th birthday I was stumped about what to get her as a gift. She has been a big influence in my life and I wanted to do something that showed that. I knew I wanted to integrate Twilio in some way because when she was younger she was a switchboard operator for Bell Telephone. Then it hit me like a ton of phones. I will create a hardware device with a Programmable Wireless SIM and a button. When she presses the button a voice call will be routed to her phone and an operator will tell her how much I love her and wish her a happy birthday. 

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

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/kwmRf0/gif03.gif"/>
</p>

I want to generate a call from my [Twilio SIM](https://www.twilio.com/console/wireless/orders/new) using the [Wio LTE board](https://www.seeedstudio.com/
-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html). A Grove Button is connected to the Wio LTE and is used for physical interaction. When the Grove Button is clicked it will trigger a callback in Twilio. To make the call I need to create some [TwiML](https://www.twilio.com/docs/voice/twiml) which will respond to the callback. Then when the call connects I need another piece of TwiML which will play a [Text-to-Speech](https://www.twilio.com/docs/voice/twiml/say/text-speech) message using [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech). Since we are connecting a SIM card on a board to a real device, we'll use a little workaround to get the message played. By including a `url` attribute on [`<Number>`](https://www.twilio.com/docs/voice/twiml/number) Twilio will play the TwiML to the person who answers the phone before they are connected. This is known as a [whisper](https://www.twilio.com/docs/glossary/call-whisper).

### Purchase a Twilio number & register a Twilio SIM

First things first, purchase a [Twilio phone number](https://www.twilio.com/console/phone-numbers/search). This phone number will be used to route the call from the [Programmable Wireless](https://www.twilio.com/docs/wireless) SIM to my grandma’s phone using voice data. 

Remove the Twilio SIM from it’s packaging, [register and activate your SIM](https://www.twilio.com/docs/wireless/tutorials/how-to-order-and-register-your-first-sim). 

### Creating the Text-to-Speech TwiML with Amazon Polly 

I wanted to create a Text-to-Speech message using [TwiML](https://www.twilio.com/docs/glossary/what-is-twilio-markup-language-twiml). TwiML is the XML-based Twilio Markup Language used to help build voice and SMS applications. A [TwiML Bin](https://www.twilio.com/console/runtime/twiml-bins) is a way to prototype an interaction with TwiML without having to create and host a web server yourself. 
To do this I used the TwiML [<Say>](https://www.twilio.com/docs/voice/twiml/say) verb to create a message that will be verbalized when my grandma answers the phone call. And to make it more realistic I used the [voice attribute](https://www.twilio.com/docs/voice/twiml/say#voice) to 
select a [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech#amazon-polly) voice to sound more lifelike.
  
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
Your TwiML should look like this when you complete the following above steps. Save the TwiML Bin and make sure to copy the URL of the TwiML Bin, it will used momentarily.

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

Great! The SIM is now configured to route an outgoing voice call with a Twilio number and read a custom message to my grandma using Text-to-Speech. That was a mouthful of cake, no doubt. Next let’s make this into a physical device. 

### Hardware setup

Unbox the [Wio LTE](https://www.seeedstudio.com/Wio-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html) board and connect the board to the computer using the Micro-USB cable provided. Depending on the operating system a [USB driver](http://wiki.seeedstudio.com/Wio_LTE_Cat.1/#install-usb-driver) may be needed to connect to the Wio LTE.

Insert the [Twilio SIM](https://www.seeedstudio.com/Twilio-Wireless-SIM-Card-p-3065.html) you registered into the Wio LTE’s SIM slot on the back of the board. 

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/b6wQOL/blog-happybirthdaygram-SIM-copy.png"/>
</p>

Within the Wio LTE box is a cellular antenna. Remove the antenna from it’s packaging and connect the antenna to the back of the Wio LTE where it says LTE main.

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/mr71V0/blog-happybirthdaygram-Antenna.png"/>
</p>

Now remove the [Grove Button](https://www.seeedstudio.com/Grove-Button-p-766.html) from it’s packaging and connect the Grove Button to the D38 socket of the Wio LTE.

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/hkyVOL/blog-happybirthdaygram-connected.jpg"/>
</p>

### Software Environment setup

Next to download [Arduino IDE](https://www.arduino.cc/en/Main/Software). This will be used to program and upload code to the Wio LTE. After installing the Arduino IDE download the [Wio LTE Arduino Library](https://github.com/Seeed-Studio/Wio_LTE_Arduino_Library) from GitHub and follow this guide to [install the library](https://github.com/Seeed-Studio/Wio_LTE_Arduino_Library#usage). The library is a wrapper for the [STM32F4 chip](https://en.wikipedia.org/wiki/STM32#STM32_F4) that utilizes the [Arduino Core](https://arduino.stackexchange.com/questions/35048/explain-what-is-meant-by-arduino-core) and adds [Quectel EC21-A](https://www.quectel.com/UploadFile/Product/Quectel_EC21_LTE_Specification_V1.0.pdf) modem functionality. 

Next put the Wio LTE into Bootloader mode. This mode needs to be enabled for uploading code to the board. To enable Bootloader mode:

1. Press and hold the BOOT0 button underneath the board

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/i1aYcf/blog-happybirthdaygram-B00-T.png"/>
</p>

2. Press the RST on the top of the board

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/iZEZHf/blog-happybirthdaygram-RST.png"/>
</p>

3. Release the BOOT0 button to enable Bootloader mode

### Creating the code

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/bvtiSf/gif02.gif"/>
</p>

Create a new Arduino sketch (File -> New).

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/nntWf0/blog-happybirthdaygram-sketch.png"/>
</p>

Start by adding the Wio LTE library file wio_tracker.h and defining a global variable for the [Grove Button](https://www.seeedstudio.com/Grove-Button-p-766.html) above the [setup()](https://www.arduino.cc/en/Reference/Setup/) function. Next create an instance of the WioTracker using the variable wio and create a counter variable. The counter will be used to [debounce](https://www.arduino.cc/en/Tutorial/Debounce) the Grove button so we have more control over the Grove button’s interaction.

```arduino
#include "wio_tracker.h"

#define BUTTON_PIN  (D38)

WioTracker wio = WioTracker();
int counter = 0;
```

The [setup()](https://www.arduino.cc/en/Reference/Setup/) function  is used to initialize various aspects of the program at startup. The [pinMode()](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/) is used to set the physical pin the Grove Button is connected to and the type of functionality the board expects. Then the board runs through it’s initialization process and connects the Twilio SIM to the cellular network.

```arduino
void setup()
{
  // GROVE Button Setup
  pinMode(BUTTON_PIN, INPUT);

  // Wio LTE Power Up
  SerialUSB.println("Wait for power on...");
  wio.Power_On();
  SerialUSB.println("Power On O.K!");

  // Wio LTE Initialization
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

The Grove Button logic for the program is created in the [loop()](https://www.arduino.cc/en/Reference/Loop?setlang=it) function. This function continually listens for changes in the state of the board. In this case it is waiting for the [button state to change](https://www.arduino.cc/en/Tutorial/StateChangeDetection). The variable buttonState reads incoming Grove Button state changes from the physical pin on the board using [digitalRead()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/).

```arduino
void loop()
{
  // Define Grove Button state
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
      wio.callUp("GRANDMA_PHONE_NUMBER");
    }
  }
  // Restart
  delay(100);
}

```

The main if-statement manages the incoming state changes. The counter is used as a way to [debounce](https://www.arduino.cc/en/Tutorial/Debounce) the Grove Button so only a single call is made. If there was no debouncing logic the program would continue to make calls every 100ms while the Grove Button is held down.

When the Grove Button is not being pressed, state “0”, the counter will reset to 0. 

```arduino
  if (buttonState == 0) {
    // Zero counter
    counter = 0;
  }
```

When the Grove Button is pressed, state “1”, the counter starts to iterate. A nested if-statement is used to catch a specific number created by the counter. When the number 1 is caught the call is made to my grandma using `wio.callUp("GRANDMA_PHONE_NUMBER")`.  

```arduino
 else {
    // Continue counter
    counter++;
    // Catch only 1 press, no duplicate calls
    if (counter == 1) {
      SerialUSB.println("Happy Birthday sent!");
      wio.callUp("GRANDMA_PHONE_NUMBER");
    }
  }

```

That's all the code we need, to transfer this to the Wio LTE board press upload. 

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/dXJa7f/blog-happybirthdaygram-upload.png"/>
</p>

When the upload finishes disconnect the Wio LTE from the Micro-USB cable. Connect the lithium battery to the board. 

### Box it up

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/iV3KYL/IMG-3799.jpg"/>
</p>

For the party I placed the board, battery and antenna in a small box to make it more presentable. I tried my best to make it fancy but I felt it looked cooler exposed with a few parts hidden. [During the event I sat down with my grandma and presented the box to her](https://www.youtube.com/watch?v=7xkZ7l0JMBI). I managed to record a short video of her demoing it for the first time and she was jazzed about it. I can’t imagine what it must be like seeing the evolution of communication from her eyes and ears. If you are curious what happened I recorded the video so people can see her reaction in real time.

### Thoughts

After creating this project I realized this could be useful for individuals who may be separated by long distances. And instead of Text-to-Speech an audio file could be used to send personal messages like voicemail in reverse. Sending a love note and reminding others you care is important. What type of #TwilioIoT projects are you building with [Programmable Wireless](https://www.twilio.com/docs/wireless)? Reach out on [Twitter](http://twitter.com/cskonopka) with your comments, questions and projects anytime. See you on the web, ride the wave.

<p align="center">
  <img width="40%" height="40%" src="https://image.ibb.co/nhYNL0/gif01.gif"/>
</p>
