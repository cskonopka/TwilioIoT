var accountSid = process.env.TWILIO_SID;
var authToken = process.env.TWILIO_AUTH;

var twilio = require('twilio');
var client = new twilio(accountSid, authToken);

client.messages.create({
    body: 'Hello from Node',
    to: '+12345678901',  // Text this number
    from: '+12345678901' // From a valid Twilio number
})
    .then((message) => console.log(message.sid));