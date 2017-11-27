"use strict";

var socketio = require('socket.io');
var io;
var dgram = require('dgram');

exports.listen = function(server) {
  io = socketio.listen(server);
  io.sockets.on('connection', function(socket) {
    handleCommand(socket);
  });
};

function handleCommand(socket) {
  var errorTimer = setTimeout(function() {
    socket.emit("error_response", "UDP socket closed");
  }, 5000);

  socket.on('udp', function(command) {

    var PORT = 12345;
    var HOST = '192.168.7.2';
    var buffer = new Buffer(command + "\n");

    var client = dgram.createSocket('udp4');
    client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
      if (err) {
        throw err;
      }

      client.on('listening', function() {
        var address = client.address();
        console.log('UDP Client: listening on ' + address.address + ":" + address.port);
      });

      client.on('message', function(message, remote) {
        console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

        var reply = message.toString('utf8');
        socket.emit('commandReply', reply);

        client.close();
        clearTimeout(errorTimer);
        errorTimer = setTimeout(function() {
          socket.emit("error_response", "Udp socket closed");
        }, 5000);
      });

      client.on("UDP Client: close", function() {
        socket.emit("error_response", "UDP closed");
      });
      client.on("UDP Client: error", function(err) {
        socket.emit("error_response", "UDP error: " + err);
      });
    });
  });
};
