"use strict";
// Client-side interactions with the browser.

// Web sockets: automatically establish a socket with the server
var socket = io.connect();

// Make connection to server when web page is fully loaded.
$(document).ready(function() {


//When the button defined at $('#modediv #volumeDown') is clicked, emit a 'decvolume'
//This is picked up by elevatorserver.js for further processing
$('#modediv #volumeDown').click(function() {

socket.emit('decvolume');

});

//Volume up
$('#modediv #volumeUp').click(function() {

socket.emit('incvolume');

});

});

//Emits a getvolume when called
  function getVolume() {
    socket.emit('getvolume');
  }

  function getTempo() {
    socket.emit('gettempo');
  }

  function getMode() {
    socket.emit('getmode');
  }

  function getUptime() {
    socket.emit('getuptime');
  }

//Calls the defined functions above at an interval of 1000ms.
  var checkvol = setInterval(getVolume, 1000);
  var checktempo = setInterval(getTempo, 1000);
  var checkmode = setInterval(getMode, 1000);
  var checkuptime = setInterval(getUptime, 1000);

//When elevatorserver_ui.js picks up a 'volume' from elevatorserver.js
//Change the value defined at $('#modediv #volumeid') to result, the data included with the volume packet recieved
socket.on('volume', function(result) {
    $('#modediv #volumeid').val(result);
})

socket.on('tempo', function(result) {
    $('#modediv #tempoid').val(result);
})


socket.on('mode', function(result) {

var mode = Number(result);

  switch (mode) {
  case 0:
    $('#modediv #modeid').text('None');
    break;
  case 1:
      $('#modediv #modeid').text('Rock Beat');
    break;
  case 2:
      $('#modediv #modeid').text('Alternative');
      break;
  default:
      break;
  }
})


socket.on('uptime', function(result) {
    $('#status #uptimeid').text(result);
})

});
