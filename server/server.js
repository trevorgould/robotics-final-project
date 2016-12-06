var fs = require('fs')
,http = require('http'),
socketio = require('socket.io'),
url = require("url"),
SerialPort = require("serialport").SerialPort

var socketServer;
var serialPort;
var portName = 'COM1'; //change this to your Arduino port
var sendData = "";

// handle contains locations to browse to (vote and poll); pathnames.
function startServer(route,handle,debug)
{
	// on request event
	function onRequest(request, response) {
	  // parse the requested url into pathname. pathname will be compared
	  // in route.js to handle (var content), if it matches the a page will
	  // come up. Otherwise a 404 will be given.
	  var pathname = url.parse(request.url).pathname;
	  console.log("Request for " + pathname + " received");
	  var content = route(handle,pathname,response,request,debug);
	}

	var httpServer = http.createServer(onRequest).listen(8080, function(){
		console.log("Listening at: http://localhost:8080");
		console.log("Server is up");
	});
	serialListener(debug);
	initSocketIO(httpServer,debug);
}

function initSocketIO(httpServer,debug)
{
	socketServer = socketio.listen(httpServer);
	if(debug == false){
		socketServer.set('log level', 1); // socket IO debug off
	}
	socketServer.on('connection', function (socket) {
	console.log("user connected");
	socket.emit('onconnection', {pollOneValue:sendData});
	socketServer.on('update', function(data) {
	socket.emit('updateData',{pollOneValue:data});
	});
	socket.on('left', function(data) {
		serialPort.write(data + 'A');
	});
	socket.on('back', function(data) {
		serialPort.write(data + 'S');
	});
	socket.on('right', function(data) {
		serialPort.write(data + 'D');
	});
	socket.on('forward', function(data) {
		serialPort.write(data + 'W');
	});
/*	socket.on('name', function(data) {
		serialPort.write(data + 'U');
	});
	socket.on('name', function(data) {
		serialPort.write(data + 'U');
	});
	//these 2 are undecided, add what you want, replace name and U 
	*/
    });
}

// Listen to serial port
function serialListener(debug)
{
    var receivedData = "";
    var SerialPort = require("serialport").SerialPort
    serialPort = new SerialPort('/dev/ttyACM0', {
        baudrate: 9600,
        // defaults for Arduino serial communication
         dataBits: 8,
         parity: 'none',
         stopBits: 1,
         flowControl: false
    });

    serialPort.on("open", function () {
      console.log('open serial communication');
            // Listens to incoming data
        serialPort.on('data', function(data) {
             receivedData += data.toString();
          if (receivedData .indexOf('F') >= 0 && receivedData .indexOf('S') >= 0) {
           sendData = receivedData .substring(receivedData .indexOf('S') + 1, receivedData .indexOf('E'));
           receivedData = '';
         }
         // send the incoming data to browser with websockets.
         //S = start indicating start of data stream to push F is finish to signify end of data
       socketServer.emit('update', sendData);
      });
    });
}

exports.start = startServer;
