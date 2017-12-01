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
    switch (command) {
      case 'angle':
        $('#angleText').html(reply);
        break;
      case 'accel':
        $('#accelText').html(reply);
        break;
      case 'help':
        $('#helpText').html(reply);
        $('#helpText').show();
        hideAfterTimeout();
        break;
      default:
        break;
    }
    $('#status').html('Node server successfully connected to beaglebone!');
    //$('#status').html(reply);
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
          $('#status').html('Connection to beaglebone server timed out.');
    var timer = setTimeout(function() {
      $('#error-box').hide();
    }, 10000);
  };

  function hideAfterTimeout() {
    var timer = setTimeout(function() {
      $('#helpText').hide();
    }, 10000);
  }
