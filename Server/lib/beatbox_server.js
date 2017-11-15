var socketio = require('socket.io');
var io;
var socket;

exports.listen = function(server) {
	io = socketio.listen(server);

	io.sockets.on('connection', function(soc) {

		handleCommand(soc);
		socket = soc;

	});

};

function handleCommand(socket) {

	socket.on('incvolume', function(data){
incVolume();
	});

socket.on('decvolume', function(data){
decVolume();
	});

	socket.on('getvolume', function(data){

getVolume();
	});

  socket.on('inctempo', function(data){
incTempo();
	});

  socket.on('dectempo', function(data){
decTempo();
	});

	socket.on('gettempo', function(data){

getTempo();
	});

	socket.on('playsound1', function(data){
playSound1();
	});

	socket.on('playsound2', function(data){
playSound2();
	});

	socket.on('playsound3', function(data){
playSound3();
	});

	socket.on('setmode0', function(data){
setMode0();
	});

	socket.on('setmode1', function(data){
setMode1();
	});

	socket.on('setmode2', function(data){
setMode2();
	});

	socket.on('getuptime', function(data){

getUptime();
	});

	socket.on('getmode', function(data){

	getMode();
	});

};

//Creates a UDP socket that connects to our BeagleBone (client)
var uPORT = 12345;
var uHOST = '192.168.7.2';
var dgram = require('dgram');
var client = dgram.createSocket('udp4');

function incVolume() {
  var message = new Buffer('incvolume');
  sendToBBG(message);
}

function decVolume() {
  var message = new Buffer('decvolume');
  sendToBBG(message);
}

function getVolume() {
  var message = new Buffer('getvolume');
  sendToBBG(message);
}

function incTempo() {
  var message = new Buffer('inctempo');
  sendToBBG(message);
}

function decTempo() {
  var message = new Buffer('dectempo');
  sendToBBG(message);
}

function getTempo() {
  var message = new Buffer('gettempo');
  sendToBBG(message);
}

//Play base sound
function playSound1() {
  var message = new Buffer('queuebase');
  sendToBBG(message);
}

//Play snare sound
function playSound2() {
  var message = new Buffer('queuesnare');
  sendToBBG(message);
}

//Play highhat sound
function playSound3() {
  var message = new Buffer('queuehihat');
  sendToBBG(message);
}

//Set drum mode 0
function setMode0() {
  var message = new Buffer('setmode0');
  sendToBBG(message);
}

//Set drum mode 1
function setMode1() {
  var message = new Buffer('setmode1');
  sendToBBG(message);
}

//Set drum mode 2
function setMode2() {
  var message = new Buffer('setmode2');
  sendToBBG(message);
}

function getMode() {
  var message = new Buffer('getmode');
  sendToBBG(message);
}

function getUptime() {
  var message = new Buffer('getuptime');
  sendToBBG(message);
}
//Sends a UDP packet to BBG

function sendToBBG(message) {
  client.send(message, 0, message.length, uPORT, uHOST, function(err, bytes) {
     if (err) throw err;

});
}

client.bind(12345, "192.168.7.1");

client.on('message', function(msg) {
  //console.log(`${msg}`);

strcast = msg.toString();

	processCommand(strcast);
});

function processCommand(msg) {

	var words = msg.split(' ');
	var operation = words[0];

	var returnValue = words[1];

	switch(operation) {

	case 'volume':
		socket.emit('volume', returnValue);
		break;

	case 'tempo':
socket.emit('tempo', returnValue);
	break;

	case 'mode':

socket.emit('mode', returnValue);
	break;
	case 'beat':

socket.emit('beat', returnValue);
	break;

case 'uptime':

socket.emit('uptime', returnValue);
	break;

default:
break;

}
}
