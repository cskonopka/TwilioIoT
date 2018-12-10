var accountSid = process.env.TWILIO_SID;
var authToken = process.env.TWILIO_AUTH;

var twilio = require('twilio');
var client = new twilio(accountSid, authToken);

client.wireless.commands
    .create({
        commandMode: 'binary',
        sim: 'AliceSmithSmartMeter',
        command: 'SGVsbG8sIE1hY2hpbmUh=='
    })
    .then(command => console.log(command.sid))
    .done();