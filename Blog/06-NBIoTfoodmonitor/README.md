# 06 - Create an NB-IoT environmental monitor for food shipments with TwilioIoT, Node and Pusher

<p align="center">
  <img width="70%" height="70%" src="https://i.ibb.co/qpk0ZMX/Twilio-Io-T-NBFood-Banner2.png"/>
</p>

[Containers](https://en.wikipedia.org/wiki/Intermodal_container) are one of the many ways food is shipped to vendors around the globe. The food encounters various types of environmental changes before it reaches the consumer. These environmental changes could impact the quality of the product during the shipment. 

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/huWcRHuN4LCQE/giphy.gif"/>
</p>

Using *[Twilio’s Alfa Developer Kit](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit)* that was distributed at [SIGNAL 2018](https://www.twilio.com/signal) we will create a full-stack IoT prototype using the [U.S. T-Mobile NB-IoT](https://www.twilio.com/docs/wireless/nb) network. The rollout of *[NB-IoT](https://www.twilio.com/wireless/narrowband)* provides a new way to create and connect devices with a lifespan of 10 years in the wild. This device will be used to track the [temperature/humidity](http://wiki.seeedstudio.com/Grove-Temperature_and_Humidity_Sensor_Pro/) of a food shipment and infrequently send small packets of data to update a web application.

<p align="center">
  <img width="40%" height="40%" src="https://www.youtube.com/watch?v=N4ZKnuvLjPU"/>
</p>

### Hardware Requirements
*  Twilio Developer Kit for T-Mobile Narrowband
* Twilio Narrowband SIM
* LTE Antenna
* Micro USB cable
* Lithium Battery
* Seeed Studio Temperature and Humidity sensor

### Software Requirements
* Arduino IDE
* Twilio Breakout SDK
* Node.js
* Pusher

## Prepare the hardware
If you are unfamiliar with the Alfa developer kit check out how “Pioneer NB-IoT with Twilio’s Alfa Development Kit”. This is a great primer for understanding how to connect to T-Mobile’s NB-IoT network with Twilio.

First take out the required components from the Alfa developer kit. 



Attach the Seeed Studio Temperature and Humidity DHT11 sensor.



Open the Arduino IDE. This developer environment is used to program the developer board. 

We will need to download the Grove Temperature and Humidity Sensor library by Seeed Studio to program the DHT11 sensor. Navigate to Sketch > Include Library Manage Libraries.



Search for and install the Grove Temperature and Humidity Sensor library by Seeed Studio. 



After the library is installed we are ready to modify some code. Before we can do this we have to put the developer board into Bootloader mode. This step is necessary for uploading new code to the board. Once it is in Bootloader mode go back to the Arduino IDE.

## Hack and upload

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/ZvLUtG6BZkBi0/giphy.gif"/>
</p>

Open the “Hello World” example in the Breakout SDK Library folder.
Navigate to File > Examples > Breakout Arduino Library > Hello World 



At the top of the file, underneath #include <stdio.h>, add the DHT11 sensor header file.

``` arduino
#include <DHT.h>
```

Define SENSOR_PIN and the type of temperature humidity sensor as DHT11. This sensor is apart of a family called DHT so it’s important to note that we are using the DHT11. Once defined now we can instantiate a new DHT instance for reading incoming data.

``` arduino
#define SENSOR_PIN (D38)
#define DHTTYPE DHT11 
DHT dht(SENSOR_PIN, DHTTYPE);
```

Navigate to the setup() function and add this line at the top of the function.

``` arduino
  dht.begin();
```
Create a new function between the setup() and loop() functions. Name this function sendDHT11. 

``` arduino
void sendDHT11() {
}
```
Within the function, create the timing logic for reading the DHT11. Add a delay at the end of the block to take a reading every minute.

``` arduino
  static unsigned long last_send = 0;

  if ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL)) {
    last_send = millis();
  }
  delay(60000);
```

Add the temperature and humidity variables to the if-statement underneath the variable last_send. 
 
``` arduino
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    LOG(L_INFO, "Current temperature [%f] degrees celcius\r\n", temperature);
    LOG(L_INFO, "Current humidity [%f]\r\n", humidity);
```

Create a new char buffer named commandText. Use the snprintf function to create a new data string with the humidity and temperature data from the DHT11 sensor. This function will place the string in the buffer and it can be pointed to by the variable commandText.

``` arduino
char commandText[512];
    snprintf(commandText, 512, "Current humidity [%4.2f] and current temp [%4.2f]", humidity, temperature);
```

This variable is then used point to the string and send a text command over the NB-IoT network using the Breakout SDK.
``` arduino
    breakout->sendTextCommand(commandText);
```

For testing purposes, there are two other things we should do 1) add the Breakout SDK psk_key and 2) alter the interval time for a more responsive example.

Let’s start by replacing the psk_key. Each development board has a unique SIM ICCID and Pre-Shared Key (psk). The psk for the board needs to be copied into the HelloWorld.ino sketch to the T-Mobile Narrowband network. Follow the instructions below to find your pre-shared key:

Inside the HelloWorld.ino sketch find the line *psk_key

1. Navigate to Programmable Wireless in the Twilio Console
2. Click SIMs
3. Find the Narrowband SIM that was previously registered
4. Under the tab Breakout SDK find Credentials
5.Where it says Pre-Shared Key (psk) click the eye logo to reveal the key
6. Copy the psk
7. Paste your psk into the HelloWorld.ino file in the code above

Next, change the setPollingInterval. This determines how often the Breakout SDK will poll for a new Command. Find the line breakout->setPollingInterval.

Change the ‘setPollingInterval’ interval from 10 to 1. This will change the polling time from 10 minutes to 1 minute.
``` arduino
breakout->setPollingInterval(1 * 60);
```
With the board still in Bootloader mode, upload the code to the developer board. 



The output of the uploading process should look something like this.



Press the RST button on the board to reset the board and start the new program. 



Open the Serial Monitor and the registration process will start. 



After a short period of time, the developer board will connect to the Breakout SDK.


Once connected to the Breakout SDK, the DHT11 will initialize and the temperature and humidity data will start to flow in the Serial Monitor. 

Below is the completed program as a reference	. This code can also be found on the TwilioIoT GitHub.
```arduino
#include <Seeed_ws2812.h>
#include <BreakoutSDK.h>
#include <stdio.h>

#include <DHT.h>
#define SENSOR_PIN (D38)
#define DHTTYPE DHT11 
DHT dht(SENSOR_PIN, DHTTYPE);

#define SEND_INTERVAL (1 * 60 * 1000)
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
  static unsigned long last_send = 0;

  if ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL)) {
    last_send = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    LOG(L_INFO, "Current temperature [%f] degrees celcius\r\n", temperature);
    LOG(L_INFO, "Current humidity [%f]\r\n", humidity);
    char commandText[512];
    snprintf(commandText, 512, "Current humidity [%4.2f] and current temp [%4.2f]", humidity, temperature);
    breakout->sendTextCommand(commandText);
  }
  delay(60000);
}

void loop() {
  your_application_example();
  breakout->spin();
  delay(50);
}
```
## Creating the project from scratch

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/Ro2MgOxH9iaVG/giphy.gif"/>
</p>

Getting more information on each shipment would provide more insight about the freshness of the food during its journey. The data from inside the shipment is sent over the NB-IoT network using the Breakout SDK. The next step is to create an application that can receive the data and use Pusher to update a web page. The completed code can be found on GitHub[link].

Create a new folder in the directory and then create a new Node application using npm.

``` node
npm init
```

We will need a few npm packages. for this project. Find package.json and paste in the follow code.

``` node
{
    "name": "TwilioIoT -NBIoT-timeline",
    "version": "0.0.2",
    "description": "Real-time temp/humidity web app using TwilioIoT and T-Mobile's NBIoT network",
    "main": "server.js",
    "scripts": {
        "test": "echo \"Error: no test specified\" && exit 1"
    },
    "author": "cskonopka",
    "license": "MIT",
    "dependencies": {
        "body-parser": "^1.16.1",
        "db": "^1.1.1",
        "dotenv": "^6.2.0",
        "express": "^4.16.4",
        "path": "^0.12.7",
        "pusher": "^2.2.0",
        "twilio": "^3.27.0"
    }
}
```
Next let’s install them.

``` node
npm i
``` 

Great! Now we’re cooking.

## How do you connect the server with the client?

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/m12EDnP8xGLy8/giphy.gif"/>
</p>

Now that the data is being sent from board using the Breakout SDK, we need a mechanism to push the data received by the server to the client. For this we will use the Pusher API. This API makes it easier for developers to integrate bi-directional communication via WebSockets for web and mobile applications. Go to Pusher and sign up for a free account.

Once the signup process is complete, Pusher will redirect to the project Dashboard. At that point observe the menu on the left side of the page.



Click Create new app to create a new Channels application. Provide a name for the app and select a cluster where the application will be hosted. Select Javascript as the front-end tech and Node.js as the back-end tech. After filling out the necessary details press Create my app.



Pusher will redirect to the Dashboard once the app is created.



Press the Channels Apps in the navigation bar on the left.



A new menu will appear with all of Channels apps associated with the account. Click the application we just created to go to the application’s dashboard.



Once at the Channels dashboard we need to copy the Pusher credentials. They can be found at the bottom of the page. These credentials will be used in a future step.



## Create the server and listen for messages

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/l41lFj8afmWIo3yW4/giphy.gif"/>
</p>

The server is the connective tissue between the outside world and the web. All of the Commands being sent over the NB-IoT network will be received using Node and Express that we will create together. Within the server, we will create a specific route to receive the data and pass to the client using Pusher. 

In the project folder we created in the previous step, create a new server file called server.js.

``` bash 
touch server.js
```
Open the file and add the following libraries.

``` javascript
var express = require('express');
var path = require('path');
var bodyParser = require('body-parser');
var Pusher = require('pusher');
```

Under the required libraries,instantiate Pusher. Find the credentials that were previously copied and add them to the file. 

``` javascript
var pusher = new Pusher({
    appId: PUSHER_APPID,
    key: PUSHER_KEY,
    secret: PUSHER_SECRET,
    cluster: PUSHER_CLUSTER,
    encrypted: true
});
```

Create a new Express instance that returns middleware that is json compatible. This will allow us to properly parse the incoming Commands from the Narrowband SIM Webhook we will set up later.

``` javascript
var app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: false
}));
```

Define the path of the client-side application. 

``` javascript
app.use(express.static(path.join(__dirname, 'public')));
```

Create a POST method route named “/fromSIM”. This route receives a json formatted request from an incoming Command via a Webhook. Within the method route the newCommand variable is used to parse the json and get the data from req.body.Command. The variable is then pushed from the server-side and to the client-side by using pusher.trigger from the Pusher library.

``` javascript 
app.post('/fromSIM', function(req, res) {
    console.log(req.body);
    var newCommand = {
        name: "Reading Received",
        command: req.body.Command
    }
    pusher.trigger('m2m-commands', 'new_command', newCommand);
    res.json({
        created: true
    });
});
``` 

Export the application using module.exports.
``` 
module.exports = app;
```

Set up the server to listen for incoming Commands on local port 9050.
``` javascript
app.listen(9050, function() {
    console.log('Example app listening on port 9050!')
});

```
Start the server.

```javascript
node server.js
```


## Creating an interactive client

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/3oz8xYfQd5358zpL0s/giphy.gif"/>
</p>

The client-side is where the Command data will be received from the server. A function will receive the data from the server, and it will dynamically create new HTML elements. These elements will display the temperature and humidity readings of the remote IoT device. 

Create a file named app.js inside the public folder. These images can be found in this projects GitHub. 

``` bash
mkdir assets
touch app.js
```

Open app.js and add a Immediately-Invoked Function Expression.

``` javascript
(function() {
})();

```

Within the function, we want to add the Pusher code that connects the front-end to the back-end. This will subscribe to a specific <div> within the HTML file and bind the data from the server to a specific HTML element.

``` javascript
Pusher.logToConsole = true;

    var serverUrl = "/",
        commands = [],
        pusher = new Pusher(PUSHER_KEY, {
            cluster: PUSHER_CLUSTER,
            encrypted: true
        }),
        channel = pusher.subscribe('m2m-commands'),
        commandList = document.getElementById('commands-list'),
        commandTemplate = document.getElementById('command-template');

    channel.bind('new_command', newCommandReceived);
```

Create a function within the main function named newCommandReceived. This function will receive the Command data from the server-side and dynamically generate new HTML elements.

``` javascript
    function newCommandReceived(data) {
    }
```

The variables {{name}} and {{command}} inside the HTML element are replaced with new temperature and humidity data reading. This is done using data.name and data.command. 

``` javascript
        var newCommandHtml = commandTemplate.innerHTML.replace('{{name}}', data.name);
        newCommandHtml = newCommandHtml.replace('{{command}}', data.command);
```

A new <div> is created using document.createElement(‘div’), and and then we append the new temperature and humidity data readings to a <div> with the class command.  

``` javascript
        var newCommandNode = document.createElement('div');
        newCommandNode.classList.add('command');
        newCommandNode.innerHTML = newCommandHtml;
        commandList.appendChild(newCommandNode);
```

This new <div> is then dynamically added to the <div> with the class commands-list.

``` javascript
        var parent = document.getElementById("commands-list");
        var nodesSameClass = parent.getElementsByClassName("command");
```

Incoming Commands will dynamically add new <div> blocks beyond the dimensions of the web page. To fix that we need to find out the number of <div> tags that have been dynamically created and limit the number available on the page. So when there are 6 <div> tags on the page, 5 will be erased and the 6th will start a new list. This is quick hack to keep everything on one page.

``` javascript
        if (nodesSameClass.length == "6") {
            for (var i = nodesSameClass.length - 2; 0 <= i; i--)
                if (nodesSameClass[i] && nodesSameClass[i].parentElement)
                    nodesSameClass[i].parentElement.removeChild(nodesSameClass[i]);
        }
```
Great work! Let’s mock up the HTML so it can be dynamically altered.

## Set up the HTML template

The HTML file is the skeleton of the web application and with the previous code it can be dynamically altered. Next, we have to create the template that will update when new readings are received.

Create a new file named index.html in the public folder.

``` bash
touch index.html
```

Below is the entire index.html file. We will not dive into every aspect of the file but we will focus on the highlighted section of the code. 

``` HTML
<!DOCTYPE>
<html>
    <head>
        <title>Twilio Narrowband</title>
        <link rel="stylesheet" href="https://unpkg.com/purecss@0.6.2/build/pure-min.css" integrity="sha384-UQiGfs9ICog+LwheBSRCt1o5cbyKIHbwjWscjemyBMT9YCUMZffs6UqUTd0hObXD" crossorigin="anonymous">
        <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js" integrity="sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut" crossorigin="anonymous"></script>
        <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js" integrity="sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k" crossorigin="anonymous"></script>
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css" integrity="sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS" crossorigin="anonymous">
        <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Verdana">
        <link rel="stylesheet" href="./style.css">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="manifest" href="/manifest.json">
        <meta name="msapplication-TileColor" content="#ffffff">
        <!-- <meta name="msapplication-TileImage" content="/ms-icon-144x144.png"> -->
        <meta name="theme-color" content="#ffffff">
    </head>
    <body>
        <div class="container-fluid">
            <section>
                <div class="m2m-commands">
                    <div class="container">
                        <div class="icon">
                            <div class="row">
                                <img style="height:35%; margin: auto;" src="./assets/twilio.png">
                            </div>
                        </div>
                        <div class="centerdiv">
                            <div class="row">
                                <div class="commands-list" id="commands-list">
                                    <script id="command-template" type="text/x-template">
                                    <div class="user-icon">
                                        <img class="imgstyle" src="./assets/check.png" />
                                    </div>
                                    <div class="command-info">
                                        <div class="row">
                                            <div class="name">{{name}}</div>
                                        </div>
                                        <div class="row">
                                            <div class="text"><em>{{command}}</em></div>
                                        </div>
                                    </div>
                                    </script>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </section>
        </div>
        <script type="text/javascript" src="https://js.pusher.com/3.2/pusher.min.js"></script>
        <script type="text/javascript" src="./app.js"></script>
    </body>
</html>
```
This section of HTML is dynamically altered by new data readings from the Breakout SDK. When the data is received by the Express server it is routed to a function and is passed to the client-side. This function dynamically adds HTML elements and the data is added to the variables {{name}} and {{command}}. When the process is complete a new block of HTML is created with the temperature/humidity reading from the remote IoT device.

``` html
<div class="commands-list" id="commands-list">
                                    <script id="command-template" type="text/x-template">
                                    <div class="user-icon">
                                        <img class="imgstyle" src="./assets/check.png" />
                                    </div>
                                    <div class="command-info">
                                        <div class="row">
                                            <div class="name">{{name}}</div>
                                        </div>
                                        <div class="row">
                                            <div class="text"><em>{{command}}</em></div>
                                        </div>
                                    </div>
                                    </script>
                                </div>
```

The stylesheet used for the web app, style.css, can be found in the TwilioIoT GitHub repository for this post.

## Connect the two worlds with a webhook

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/gw3IWyGkC0rsazTi/giphy.gif"/>
</p>

Connecting the Breakout SDK to the Node server is the last step needed to connect all of the applications elements. 

When the device sends a machine-to-machine Command to Twilio a webhook is sent to the Commands Callback Url. Ngrok will be used to receive the webhook and  route it to the server running locally. 

Start a new ngrok instance on the same port where the server is running. 

``` ngrok http 9050 ```

Copy the Forwarding url that was created with ngrok. Note that the Forwarding url will change each time ngrok is started. This behavior can be changed by defining a subdomain using a command-line flag.

```
http://3c2a36e0.ngrok.io
```



Next we want to add Forwarding url to the Commands Callback Url. 

Navigate to Programmable Wireless in the Twilio console
Locate the Narrowband SIM that you previously registered under SIM
Under the Configure tab you will find the Commands Callback Url
Paste the ngrok Forwarding address into text box and add the previously created server route to the end of the url.

``` http://3c2a36e0.ngrok.io/fromSIM ```



Press save. 

We are connected, let’s test it out!

## Time to check the shipment

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/d31w24psGYeekCZy/giphy.gif"/>
</p>

Open a web browser and type in localhost:9050.
 


Watch the machine-to-machine Commands reach the Node server.



And this is what it looks like when the data reaches the web browser.



You just create a full-stack IoT prototype!

<p align="center">
  <img width="40%" height="40%" src="https://media.giphy.com/media/d31w24psGYeekCZy/giphy.gif"/>
</p>

## Prototyping the future of IoT

IoT is not just software, hardware, or even cloud technology. It is about interconnectivity with intent. Developing experimental prototypes like this “smart container app” is how an individual could bring their idea and create an MVP. After several iterations it could potentially become a production ready device. 

Environment tracking could be applied to several types of industries such as:
Smart Buildings
Precision agriculture
Smart Manufacturing
This project, along with other projects, can be found on the TwilioIoT GitHub. If you are interested in learning about other pieces of hardware that can send machine-to-machine (M2M) Commands with TwilioIoT check out the Wireless Machine-to-Machine Quickstarts. 


https://media.giphy.com/media/2mUE54iv9wPJu/giphy.gif
Feel free to reach out with any questions or curiousity. Drop me a line if you have any cool IoT projects.
Email: ckonopka@twilio.com
Github: cskonopka
Twitter: @cskonopka
