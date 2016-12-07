var fs = require('fs'),
  http = require('http'),
  socketio = require('socket.io'),
  url = require("url"),
  SerialPort = require("serialport");

var socketServer;
var serialPort;
var portName = 'tty.usbmodem1411';
var sendData = "";

// handle contains locations to browse to (vote and poll); pathnames.
function startServer(route, handle, debug) {
  // on request event
  function onRequest(request, response) {
    // parse the requested url into pathname. pathname will be compared
    // in route.js to handle (var content), if it matches the a page will
    // come up. Otherwise a 404 will be given.
    var pathname = url.parse(request.url).pathname;
    console.log("Request for " + pathname + " received");
    var content = route(handle, pathname, response, request, debug);
  }

  var httpServer = http.createServer(onRequest).listen(8080, function() {
    console.log("Listening at: http://localhost:8080");
  });
  serialListener(debug);
  initSocketIO(httpServer, debug);
}

function initSocketIO(httpServer, debug) {
  socketServer = socketio.listen(httpServer);
  if (debug == false) {
    socketServer.set('log level', 1); // socket IO debug off
  }
  socketServer.on('connection', function(socket) {
    console.log("Client connected!");

    socket.emit('connected');

    // * Movement
    socket.on('moveForward', function(data) {
      console.log('moveForward');
      serialPort.write('W');
    });

    socket.on('moveBackward', function(data) {
      console.log('moveBackward');
      serialPort.write('S');
    });

    socket.on('moveLeft', function(data) {
      serialPort.write('A');
    });

    socket.on('moveRight', function(data) {
      serialPort.write('D');
    });

    // * Servo
    socket.on('servoLeft', function(data) {
      console.log('servoLeft');
      serialPort.write('L');
    });

    socket.on('servoRight', function(data) {
      console.log('servoRight');
      serialPort.write('R');
    });
  });
}

// Listen to serial port
function serialListener(debug) {
  var receivedData = "";
  var SerialPort = require("serialport")
  serialPort = new SerialPort('/dev/tty.usbmodem1421', {
    baudrate: 9600,
    // defaults for Arduino serial communication
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
  });

  serialPort.on("open", function() {
    console.log('Opening serial communication...');
    // Listens to incoming data
    serialPort.on('data', function(data) {
      receivedData += data.toString();
      console.log(receivedData);
      if (receivedData.indexOf('F') >= 0 && receivedData.indexOf('S') >= 0) {
        sendData = receivedData.substring(receivedData.indexOf('S') + 1, receivedData.indexOf('E'));
        receivedData = '';
      }
      // send the incoming data to browser with websockets.
      //S = start indicating start of data stream to push F is finish to signify end of data
      socketServer.emit('update', sendData);
    });
  });
}

exports.start = startServer;
