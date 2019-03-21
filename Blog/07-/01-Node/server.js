require('dotenv').load();
var express = require('express');
var path = require('path');
var bodyParser = require('body-parser');
var Pusher = require('pusher');

var pusher = new Pusher({
    appId: "PUSHER_APPID",
    key: "PUSHER_KEY",
    secret: "PUSHER_SECRET",
    cluster: "PUSHER_CLUSTER",
    useTLS: false
});

var app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: false
}));
app.use(express.static(path.join(__dirname, 'public')));

app.post('/fromSIM', function(req, res) {
    console.log("Command received from remote NB-IoT device")
    var newCommand = {
        name: "Reading Received",
        command: req.body.Command
    }
    pusher.trigger('m2m-commands', 'new_command', newCommand);
    res.json({
        created: true
    });
});

module.exports = app;

app.listen(9050, function() {
    console.log('Example app listening on port 9050!')
});