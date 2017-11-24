//Boilerplate code from 433 node.js notes
var http = require('http');

//Creates an http server with a function to deal with certain url requests
var server = http.createServer(function(request, response) {
  var filePath = false;
  if (request.url == '/') { filePath = 'public/index.html';
  } else {
    filePath = 'public' + request.url;
  }
  var absPath = './' + filePath;
  serveStatic(response, absPath);
});

var servPORT = 8088;

//Starts the server, on PORT and logs a message to the console.
server.listen(servPORT, function() {
  console.log("Server listening on port " + servPORT);
});

//More boilerplate code given to us by the instructor
var fs = require('fs');

function serveStatic(response, absPath) {

fs.exists(absPath, function(exists) {
  if (exists) {

    fs.readFile(absPath, function(err, data) {
      if (err) {
        send404(response);
      } else {
        sendFile(response, absPath, data);
      }
});

    } else {
      send404(response);
    }
  });

}

function send404(response) {
  response.writeHead(404, {'Content-Type': 'text/plain'});
  response.write('Error 404: resource not found.');
  response.end();
}

var mime = require('mime');
var path = require('path');

function sendFile(response, filePath, fileContents) {
  //was mime.lookup, renamed to getType(). Might not compile depending on different versions.
  response.writeHead(200, {"content-type": mime.getType(path.basename(filePath))});

response.end(fileContents);
}



var beatServer = require('./lib/beatbox_server');
beatServer.listen(server);
