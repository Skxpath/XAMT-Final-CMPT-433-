"use strict";

var socket = io.connect();

$(document).ready(function() {
  $('#error-box').hide();

  // Updating accelerometer data every second
  window.setInterval(function() {
    sendUDP('angle');
    sendUDP('accel');
    sendUDP('distance');
    sendUDP('stabledist');
    sendUDP('stabledist');
	sendUDP('lights');
  }, 1000);

  $('#stopButton').click(function() {
    sendUDP('stop');
  });

  socket.on('commandReply', function(reply) {
    $('#error-box').hide();
    var args = reply.split(' ');
    var command = args[0];
    switch (command) {
      case 'angle':
        $('#angleText').html("<b>Current Elevator Angle: </b>" + args[1] + "°");
        if (parseInt(args[1]) > 15) {
          warning();
        } else {
          allGood();
        }
        break;
      case 'accel':
      $('#accel-line').html("<b>Current Accelerometer Data: </b>");
      $('#xData').html("<b>X-Axis: </b>" + args[1] + "g");
      $('#yData').html("<b>Y-Axis: </b>" + args[2] + "g");
      $('#zData').html("<b>Z-Axis: </b>" + args[3] + "g");
        break;
      case 'distance':
      $('#distText').html("<b>Current Distance: </b>" + args[1] + "cm");
        break;
      case 'stabledist':
      $('#stabdistText').html("<b>Current Stable Distance: </b>" + args[1] + "cm");
        break;
	case 'lights':
      $('#lightingText').html("<b>Current lighting kit status: </b>" + args[1]);
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
}

function hideAfterTimeout() {
  var timer = setTimeout(function() {
    $('#helpText').hide();
  }, 10000);
}

function warning() {
  $('#angleWarning').html('A dangerous tilt detected! Please inspect.');
  $('#left-box').css({"background-color": "red"});
  $('#status-box').css({"background-color": "red"});
}

function allGood() {
  $('#angleWarning').html('');
  $('#left-box').css({"background-color": "green"});
  $('#status-box').css({"background-color": "#f3f3f3"});
}
