/* 
function-Sms2Sim.js

Synopsis:
This Twilio Function is meant to receive a SMS body and route it to a predefined SIM.

Setup
- Select your Twilio Phone Number
- Trigger this Twilio Function when "a message comes in" to this phone number

*/ 

exports.handler = function(context, event, callback) {
    var client = context.getTwilioClient()

    console.log(event.Body);
    client.wireless.commands.create({sim: 'Adafruit3G', command: event.Body})

   function cb() {
       callback(null, "Command Sent");
    }
    setTimeout(cb, 2000); 
};