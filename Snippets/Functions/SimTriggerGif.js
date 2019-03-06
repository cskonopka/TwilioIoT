exports.handler = function(context, event, callback) {
    var client = context.getTwilioClient()

    if (event.Command == 'function'){
        client.messages.create({to: 'TO_NUMBER', from: 'FROM_NUMBER', body: 'sent you a gif', mediaUrl:'GIF_URL'})
    }else{
        
    }
   
   function cb() {
       callback(null, "OK");
    }
    setTimeout(cb, 2000); 
};