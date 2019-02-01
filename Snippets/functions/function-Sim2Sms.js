/* 
function-Sms2Sim.js

Synopsis:
This Twilio Function is meant to 

Setup
- Select the Twilio SIM
- Apply to the Twilio Function's URL to the SIM's Commands Callback URL
- When the SIM sends a Command using "2936" a new SMS will be sent to the predefined number

*/ 

exports.handler = function(context, event, callback) {
    var client = context.getTwilioClient()
    client.messages.create({to: "PERSONAL_NUMBER", from: "TWILIO_NUMBER", body: event.Command})

   function cb() {
       callback(null, "OK");
    }
    setTimeout(cb, 3000); 
};