/*
Code workflow:
UI has buttons which when clicked emit a packet (elevatorserver_ui.js)
This packet is picked up by elevatorserver.js, which then processes the information and sends the packet to our BBG server
elevatorserver.js handles both listening and sending to the BBG code.
 */

var socketio = require('socket.io');
var io;
var socket;

//UDP Socket information to connect to our BBG.
//This is the destination we are trying to send to from our server (The address:port of the BBG)
//TODO: Change to TCP
var uPORT = 17433;
var uHOST = '192.168.7.2';

//Definition of our socket to send and listen to messages from the BBG. (Where the BBG sends to, so we can hear it)
var net = require('net');
var client = new net.Socket();
client.connect(uPORT, uHOST, function() {

    console.log('CONNECTED TO: ' + uHOST + ':' + uPORT);
    // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
    client.write('help');

});

client.on('message', function(msg) {
  console.log(`${msg}`);

strcast = msg.toString();

	processCommand(strcast);

});

//Sends a UDP packet to BBG
function sendToBBG(message) {
	client.write(message);
  //client.send(message, 0, message.length, uPORT, uHOST, function(err, bytes) {
    // if (err) throw err;
//});
}

//exports a listen function that can be called in other files to start our server.
//This is called in server.js
exports.listen = function(server) {
	io = socketio.listen(server);

	io.sockets.on('connection', function(soc) {

		handleCommand(soc);
		socket = soc;

	});
};

//When the UI gets a click on a button, it emits a packet
//We handle the emitted packet here
function handleCommand(socket) {

socket.on('incvolume', function(data){
incVolume();
	});

socket.on('decvolume', function(data){
decVolume();
	});

};

//Sends a packet to the BBG when this function is called
function incVolume() {
	//var message = new Buffer('incvolume');
	//var message = new Buffer('help');
  sendToBBG('help');
}

function decVolume() {
  var message = new Buffer('decvolume');
  sendToBBG(message);
}

//Called when our socket defined above recieves a message
//This message is sent from our BBG, we process it and emit a packet with given string, and returnValue accordingly
//The string and return value is picked up and processed in beatbox_ui.js, which then updates the UI
function processCommand(msg) {

	var words = msg.split(' ');
	var operation = words[0];

	var returnValue = words[1];

	switch(operation) {

	case 'volume':
		socket.emit('volume', returnValue);
		break;

		default:
		break;
	}
}
