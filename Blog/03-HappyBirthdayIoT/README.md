## 03-HappyBirthdayIoT

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

I want to generate a call from my [Twilio SIM](https://www.twilio.com/console/wireless/orders/new) using the [Seeed Studio Wio board](https://www.seeedstudio.com/Wio-LTE-US-Version-4G%2C-Cat.1%2C-GNSS%2C-JavaScript%28Espruino%29-Compatible-p-2957.html). A button is connected to the Wio and is used for physical interaction. When the button is clicked it will trigger a callback in Twilio. To make the call I need to create some [TwiML](https://www.twilio.com/docs/voice/twiml) which will respond to the callback. Then when the call connects I need another piece of TwiML which will play a [Text-to-Speech](https://www.twilio.com/docs/voice/twiml/say/text-speech) message using [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech). Since we are connecting a SIM card on a board to a real device, we'll use a little workaround to get the message played. By including a `url` attribute on `[<Number>](https://www.twilio.com/docs/voice/twiml/number)` Twilio will play the TwiML to the person who answers the phone before they are connected. This is known as a [whisper](https://www.twilio.com/docs/glossary/call-whisper).

### Purchase a Twilio number & register a Twilio SIM

First things first, purchase a [Twilio phone number](https://www.twilio.com/console/phone-numbers/search). This phone number will be used to route the call from the [Programmable Wireless](https://www.twilio.com/docs/wireless) SIM to my grandmother’s phone using voice data. 

Remove the Twilio SIM from it’s packaging, [register and activate your SIM](https://www.twilio.com/docs/wireless/tutorials/how-to-order-and-register-your-first-sim). 

###Creating the Text-to-Speech TwiML with Amazon Polly 

I wanted to create a Text-to-Speech message using [TwiML](https://www.twilio.com/docs/glossary/what-is-twilio-markup-language-twiml). TwiML is the XML-based Twilio Markup Language used to help build voice and SMS applications. A [TwiML Bin](https://www.twilio.com/console/runtime/twiml-bins) is a way to prototype an interaction with TwiML without having to create and host a web server yourself. 
To do this I used the TwiML [<Say>](https://www.twilio.com/docs/voice/twiml/say) verb to create a message that will be verbalized when my grandmother answers the phone call. And to make it more realistic I used the [voice attribute](https://www.twilio.com/docs/voice/twiml/say#voice) to 
select a [Amazon Polly](https://www.twilio.com/docs/voice/twiml/say/text-speech#amazon-polly) voice so it sounded more realistic.
