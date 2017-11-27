"use strict";

var socket = io.connect();

$(document).ready(function() {
  $('#error-box').hide();
  $('#volumeid').val("80");
  $('#tempoid').val("120");
  // Updating uptime every second
  window.setInterval(function() {
    sendUDP('angle');
    sendUDP('accel');
  }, 1000);

  $('#modeNone').click(function() {
    sendUDP('help');
  });
  $('#modeCustom').click(function() {
    sendUDP('stop');
  });

  socket.on('commandReply', function(reply) {
    $('#error-box').hide();
    var args = reply.split(' ');
    var command = args[0];
    $('#status').html(args[1]);
  });

  socket.on('disconnect', function() {
    displayError("Node Server down");
  });

  socket.on('error_response', function(reply) {
    displayError(reply);
  });

});

function sendUDP(command) {
  socket.emit('udp', command);
}

function displayError(reply) {
  $('#error-text').html(reply);
    $('#error-box').show();
    var timer = setTimeout(function() {
      $('#error-box').hide();
    }, 10000);
  };
