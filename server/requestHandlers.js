

var fs = require('fs'),
server = require('./server');

function sendInterface(response) {
  console.log("Request handler 'interface' was called.");
  response.writeHead(200, {"Content-Type": "text/html"});
  var html = fs.readFileSync(__dirname + "robotics-final-project/app/index.html")
  response.end(html);
}

exports.sendInterface = sendInterface;
