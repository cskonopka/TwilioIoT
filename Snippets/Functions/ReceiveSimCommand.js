exports.handler = function(context, event, callback) {
  let response = { get_started: true };

  console.log(event.Command);
  
  callback(null, response);
};