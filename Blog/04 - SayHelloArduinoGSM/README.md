# 04 - Say hello to the Arduino MKR GSM 1400 with Programmable Wireless and Go

<p align="center">
  <img width="70%" height="70%" src="https://i.ibb.co/jRgZ01P/mkr1400.jpg"/>
</p>

The new [MKR family](https://maker.pro/arduino/tutorial/an-introduction-to-arduinos-mkr-family-and-iot-development-boards) of Arduino boards are going to change the landscape of rapid prototyping IoT solutions. The [Arduino MKR GSM 1400](https://www.arduino.cc/en/Guide/MKRGSM1400) is a great solution for anyone looking to expand the scope of their IoT projects using cellular connectivity. By integrating a modem with a microcontroller a new all-in-one communication solution has started to emerge. This paired with the [Twilio Programmable Wireless SIM](https://www.amazon.com/Twilio-001-Starter-Pack-SIMs/dp/B07C8T3QDT) makes it possible to communicate around the globe using Machine-to-Machine commands. “Things” can now be connected in ways previously impossible with WiFi or Bluetooth.

This tutorial demonstrates how to send a Machine-to-Machine Command from the [Arduino MKR GSM 1400](https://www.arduino.cc/en/Guide/MKRGSM1400) to a server written in Go. When the Machine-to-Machine Command is received server-side an audio file will play a .mp3 saying “hello”. The completed project can be found on GitHub.

## What is the Arduino MKR GSM 1400?
The Arduino MKR GSM 1400 is a development board that combines the functionality of the [Arduino Zero](https://store.arduino.cc/usa/arduino-zero) with global [GSM](https://en.wikipedia.org/wiki/GSM) connectivity using the [u-blox SARAU201 modem](https://www.u-blox.com/en/product/sara-u2-series). Traditionally communicating with a [modem](https://www.explainthatstuff.com/modems.html) is done using [AT commands](https://en.wikipedia.org/wiki/Hayes_command_set) using a separate module. This model board ships with a library that makes AT commands more accessible via function calls.


### Hardware Requirements

* [Twilio Programmable Wireless SIM](https://www.amazon.com/Twilio-001-Starter-Pack-SIMs/dp/B07C8T3QDT)
* [Arduino MKR GSM 1400](https://store.arduino.cc/usa/mkr-gsm-1400)
* GSM Antenna
* Micro USB cable

### Software Requirements

* [Arduino IDE](https://www.arduino.cc/en/Main/Software)
* [Go](https://golang.org/doc/install)
* [Beep library for Go](https://github.com/faiface/beep)
* [ngrok](http://ngrok.com/)

## Setting up the Twilio SIM

<p align="center">
  <img width="47%" height="47%" src="https://i.ibb.co/R4j1jkr/Twilio-SIM-width-500.png"/>
</p>

Remove the Twilio SIM from it’s packaging. Next [register and activate your SIM](https://www.twilio.com/docs/wireless/tutorials/how-to-order-and-register-your-first-sim) in the Twilio Console.

## Software side of things

Before programming the hardware we need to install a few pieces of software to make it work. To be able to send M2M Commands using the on-board modem we will need the [MKRGSM](https://github.com/arduino-libraries/MKRGSM) library.

Open the Arduino IDE and go to Sketch > Manage Libraries. This is where Arduino and 3rd party libraries can be installed into the Arduino IDE.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/LSc9wwt/arduinogsm-managerlibrary.png"/>
</p>

When the Library Manager window pops up search for the MKRGSM library and press install. The MKRGSM library wraps [AT commands](https://en.wikipedia.org/wiki/Hayes_command_set) into functions, making it easier to communicate with the modem. It’s phonetabulous trust me.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/TrFDtBs/arduinogsm-librarymanager.png"/>
</p>

After the library is installed we need to install the Arduino MKR GSM 1400 board cores. The Arduino MKR GSM 1400 uses a different chipset than traditional Arduinos that use [AVR](https://en.wikipedia.org/wiki/AVR_microcontrollers) [ATmega](https://engineering.eckovation.com/arduino-architecture-explained/) chipsets. This board uses the [SAMD21 Cortex-M0+](https://www.avdweb.nl/arduino/samd21/sam-d21) and it requires a different set cores. The cores do not come with the Arduino IDE and they are needed for the computer to recognize the board when connected.

Locate the Board Manager under Tools > Board > Board Manager.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/QXgZ0X0/Screen-Shot-2018-11-16-at-10-40-44-AM.png"/>
</p>

When the Board Manager window appears search for the Arduino SAMD Boards and install the cores.  

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/cDC99cT/arduinogsm-boardmanager.png"/>
</p>

Restart the Arduino IDE to complete the installation.

Great! Time to move on to the hardware setup.

## Hardware side of things
To send M2M Commands over the network we need to install the Twilio SIM. Break out the Micro SIM from the Twilio SIM card

<p align="center">
  <img width="65%" height="65%" src="https://i.ibb.co/zR6zTz1/arduinogsm-Sim-Size.png"/>
</p>

Insert the Twilio SIM into the SIM slot underneath the board.

<p align="center">
  <img width="65%" height="65%" src="https://i.ibb.co/qFWS7mz/arduinogsm-sim.jpg"/>
</p>

Next, attach the GSM antenna to the board.

<p align="center">
  <img width="65%" height="65%" src="https://i.ibb.co/2cnnmpD/arduinogsm-antenna.jpg"/>
</p>

Connect the board to the computer using a Micro-USB cable and you are geared up to connect to the network.

## Creating the Arduino sketch

<p align="center">
  <img width="35%" height="35%" src="https://i.ibb.co/MZ1B5GJ/arduinogsm-gif01.gif"/>
</p>

In the Arduino IDE create a new Arduino sketch (File > New). A template is provided that look something like this.

``` arduino
void setup(){

}

void loop(){

}
```

Instantiate the base class GSM for all of the GSM functions. To send and receive SMS messages the [GSM SMS](https://www.arduino.cc/en/Reference/GSMSMSConstructor) class needs to be instantiated as well. This happens before the setup() function.

```arduino
#include <MKRGSM.h>

GSM gsmAccess;
GSM_SMS sms;
```

In the setup() function create a [serial connection](https://en.wikipedia.org/wiki/Serial_communication) with a baud of 115200. The [baud](https://en.wikipedia.org/wiki/Baud) rate determines the speed of data over a specific communication channel.

``` arduino
Serial.begin(115200);
```

Use the [gsmAccess.begin()](https://www.arduino.cc/en/Reference/GSMBegin) function to connect to the cellular network that is identified on the Twilio SIM.

```arduino
gsmAccess.begin();
Serial.println("GSM initialized");
```

In the loop() function define the phone number where the M2M Command will be sent using the [beginSMS](https://www.arduino.cc/en/Reference/MKRGSMSMSBeginSMS) function. The number we will use is [“2936”](https://www.twilio.com/docs/wireless/tutorials/iot-guides/how-to-send-machine-machine-commands#what-is-this-2936-number). This is a special Twilio shortcode that is reserved for exchanging M2M Commands between Twilio SIMs. It uses the SMS transport to send M2M Commands over a cellular network. When a Twilio SIM creates a M2M Command a Webhook is generated, we will discuss this shortly.

``` arduino
sms.beginSMS("2936");
```

Pass a [char array](https://www.arduino.cc/reference/en/language/variables/data-types/array/) to the function [sms.print()](https://www.arduino.cc/en/Reference/GSMSMSPrint) to create a new message to be queued.

```arduino	
sms.print("hello world");
Serial.println(“Sending M2M Command”);
```

After a message is created and queued use the [endSMS()](https://www.arduino.cc/en/Reference/GSMSMSEndSMS) function to tell the modem the process is complete. Once this happens the “hello world” message will then be sent.

```arduino
sms.endSMS();
Serial.println("M2M Command Sent!");
```

The last bit of code is a while loop that will capture the program and place it in an infinite loop. The purpose of this is to ensure the M2M Command is only sent once.

``` arduino
while(1) {
    	delay(4000);
}
```

Completed sketch:

```arduino
#include <MKRGSM.h>

GSM gsmAccess;
GSM_SMS sms;

void setup(){
	Serial.begin(115200);

	gsmAccess.begin();
	Serial.println("GSM initialized");

	sms.beginSMS("2936");

        sms.print("hello world");
        Serial.println(“Sending M2M Command”);

       sms.endSMS();
       Serial.println("M2M Command Sent!");

       while(1) {
               delay(4000);
       }  
}
```

Double check that the board has been selected under Tools > Board. If it is not selected the compiler will throw an error when you try to upload the code.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/RyvNGG8/arduinogsm-selectboard.png"/>
</p>

Save the new sketch as "SayHelloArduinoGSM.ino". Before uploading the new sketch to the board let’s create a server to receive the M2M Command using Go.

## Spinning up an audio response server with Go and Beep

<p align="center">
  <img width="47%" height="47%" src="https://i.ibb.co/3hYQtSz/arduinogsm-gif02.gif"/>
</p>

Create a new Go program named “SayHelloArduinoGSM.go” using the template below.

``` go
package main

import ( 

)

func main(){
}
```

Next add the following libraries to the import section. This is where you link external libraries like Beep to a Go program.

```go
package main

import (
        "fmt"
	"github.com/faiface/beep"
	"github.com/faiface/beep/mp3"
	"github.com/faiface/beep/speaker"
	"log"
	"net/http"
	"os"
	"time"
)
```

Inside the main function create a new server route using [HandleFunc()](https://golang.org/pkg/net/http/#HandleFunc) from the [net/http](https://golang.org/pkg/net/http/) library. This will generate a new server-side route (“/helloworld”) for receiving M2M Commands from the “2936” shortcode. When an M2M Command is received it will then be funneled to the helloworld function. Open up a port and listen for incoming connections using the [ListenAndServe()](https://golang.org/pkg/net/http/#ListenAndServe) function on port 9999.

```go
func main(){
	http.HandleFunc("/helloworld", helloworld)
	http.ListenAndServe(":9999", nil)
}
```

Fantastic. Now we have to create the helloworld function. The HTTP request received by this function will be represented by the [http.Request](https://golang.org/pkg/net/http/#Request) type.

```go
func helloworld(w http.ResponseWriter, r *http.Request) {

}
```

When the request is received the M2M Command needs to be parsed. Use the [ParseForm()](https://golang.org/pkg/net/http/#Request.ParseForm) function to parse the request body as a form.

```go
	if err := r.ParseForm(); err != nil {
		log.Printf("Error parsing form: %s", err)
		return
	}
```

The data from the body can be extracted using the [PostFormValue()](https://golang.org/pkg/net/http/#Request.PostFormValue) function by passing it a key. The key will give you the value associated with the named component in the JSON response. In this case we are looking for the value of the “Command” key.

```go
	pwCommand := r.PostFormValue("Command")
	fmt.Println("pwCommand : ", pwCommand)
```

And to add a little spice let’s at some Beep code to play an audio file through your system’s audio when the Command successfully reaches the server.

```go
	f, err := os.Open("hello.mp3")
	if err != nil {
		log.Fatal(err)
	}
	s, format, _ := mp3.Decode(f)
	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))
	playing := make(chan struct{})
	speaker.Play(beep.Seq(s, beep.Callback(func() {
		close(playing)
	})))
	<-playing
```

Complete program:

```go
package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/faiface/beep"
	"github.com/faiface/beep/mp3"
	"github.com/faiface/beep/speaker"
)

func main() {
	http.HandleFunc("/helloworld", helloworld)
	http.ListenAndServe(":9999", nil)
}

func helloworld(w http.ResponseWriter, r *http.Request) {
	if err := r.ParseForm(); err != nil {
		log.Printf("Error parsing form: %s", err)
		return
	}

	pwCommand := r.PostFormValue("Command")
	fmt.Println("incoming Command from Arduino MKR GSM 1400 : ", pwCommand)

	fmt.Println("Playing audio file!")
	f, err := os.Open("hello.mp3")
	if err != nil {
		log.Fatal(err)
	}
	s, format, _ := mp3.Decode(f)
	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/10))
	playing := make(chan struct{})
	speaker.Play(beep.Seq(s, beep.Callback(func() {
		close(playing)
	})))
	<-playing
}
```

Start the server.

```go	
go run SayHelloArduinoGSM.go
```

## Constructing the bridge with ngrok

Currently the hardware and software pieces exist individually. ngrok will be used to bridge the gap.

<p align="center">
  <img width="47%" height="47%" src="https://i.ibb.co/FBY14WF/arduinogsm-gif04.gif"/>
</p>

When the SIM sends a M2M Command to Twilio a Webhook is sent to a user-defined url called the [Commands Callback Url](https://www.twilio.com/docs/wireless/api/sim#instance-post-parameters-optional). We will use ngrok to receive this Webhook and then route it to the server running on our own machine. To make the connection, start a new ngrok instance on the same port where the server is running.

``` bash
ngrok http 9999
```

Copy the Forwarding url that was created with ngrok (http://xxxxxxxx.ngrok.io)

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/hd21bwv/arduinomkrgsm-ngrok-copy.png"/>
</p>

Navigate to [Programmable Wireless in the Twilio console](https://www.twilio.com/console/wireless/simshttps://www.twilio.com/console/wireless/sims). Locate the SIM that you previously registered under SIMs. Under the Configure tab you will find the Commands Callback Url. Paste the ngrok Forwarding address into text box and add the previously created server route to the end of the url.

```bash
http://xxxxxxxx.ngrok.io/helloworld 
```

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/Bz8trVJ/arduinomkrgsm-commandscallbackurl.png"/>
</p>

Press Save.

## Now it’s time to eat some chips

Go back to the Arduino IDE and press upload.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/8KdXbzW/Screen-Shot-2018-11-19-at-4-44-14-PM-copy.png"/>
</p>

Once uploaded, double check to see if the Command was sent properly using the Serial Monitor.

* Navigate to Tools > Serial Monitor

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/0QbWCr3/arduinogsm-send.png"/>
</p>

Once the M2M Command is sent from the “2936” shortcode it is then routed to ngrok and onto the go application using the Commands Callback Url.

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/xgm7VxH/arduinogsm-ngrok2.png"/>
</p>

And finally the M2M Command reaches the server and the “helloworld.mp3”

<p align="center">
  <img width="100%" height="100%" src="https://i.ibb.co/zncZJMW/arduinogsm-go.png"/>
</p>

[video](https://www.youtube.com/watch?v=LZj-rCbhNcA)

Celltactular!

## Continue to connect things
You just sent your first M2M Command using magic.

<p align="center">
  <img width="50%" height="50%" src="https://i.ibb.co/yYkv6zq/arduinogsm-gif05.gif"/>
</p>

This M2M Command model is a foundational piece of how to use Twilio to send M2M Commands from a remote hardware device. With the integrated modem and software for sending AT commands as functions, it makes the Arduino MKR GSM 1400 an ideal piece for any IoT prototyping kit.

If you are interested in learning about other pieces of hardware that can send M2M Commands check out the [Wireless Machine-to-Machine Quickstarts](https://www.twilio.com/docs/wireless).

Feel free to reach out with any questions or curiousity. If you have any cool projects you have built or are planning on build drop me a line anytime.

* Email: ckonopka@twilio.com
* Github: cskonopka
* Twitter: @cskonopka
