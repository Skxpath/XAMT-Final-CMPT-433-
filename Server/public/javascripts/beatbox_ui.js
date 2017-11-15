"use strict";
// Client-side interactions with the browser.

// Web sockets: automatically establish a socket with the server
var socket = io.connect();

// Make connection to server when web page is fully loaded.
$(document).ready(function() {

/*
$('#error-box').show();
$('#error-box #error-text').text('vol down');
$('#error-box #error-text').text('vol up');

*/
//Volume down
$('#modediv #volumeDown').click(function() {

socket.emit('decvolume');

});

//Volume up
$('#modediv #volumeUp').click(function() {

socket.emit('incvolume');
});

//Tempo down
$('#modediv #tempoDown').click(function() {

socket.emit('dectempo');
});

//Tempo up
$('#modediv #tempoUp').click(function() {

socket.emit('inctempo');
});

//Drum sound #1
$('#modediv #sound1').click(function() {

socket.emit('playsound1');
});

//Drum sound #2
$('#modediv #sound2').click(function() {

socket.emit('playsound2');
});

//Drum sound #3
$('#modediv #sound3').click(function() {

socket.emit('playsound3');
});

//Mode #1 - None
$('#modediv #modeNone').click(function() {
socket.emit("setmode0");

});

//Mode #2 - Rock Beat
$('#modediv #modeRock1').click(function() {
socket.emit("setmode1");

});

//Mode #3 - Alternative
$('#modediv #modeRock2').click(function() {
socket.emit("setmode2");

});



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

  var checkvol = setInterval(getVolume, 1000);
  var checktempo = setInterval(getTempo, 1000);
  var checkmode = setInterval(getMode, 1000);
  var checkuptime = setInterval(getUptime, 1000);

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
