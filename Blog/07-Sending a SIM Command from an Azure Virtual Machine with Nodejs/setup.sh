#!/bin/bash
mkdir commands
cd commands
sudo apt-get install nodejs
sudo apt-get install npm
npm i twilio
touch sendcommand.js
echo "
const accountSid = 'TWILIO_SID';
const authToken = 'TWILIO_AUTH';
const client = require('twilio')(accountSid, authToken);
client.wireless.commands
           .create({sim: 'TWILIO_SIM', command: 'wakeup'})
           .then(command => console.log(command.sid))
" >> sendcommand.js