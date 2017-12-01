"use strict";

var http = require('http');
var fs = require('fs');

var server = http.createServer(function(request, response) {
  var filePath = false;
  if (request.url == '/') {
    filePath = 'public/index.html';
  } else {
    filePath = 'public' + request.url;
  }
  var absPath = './' + filePath;
  serveStatic(response, absPath);
});

var PORT = 8088;
server.listen(PORT, function() {
  console.log("Server listeneing on port " + PORT);
});

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
  response.writeHead(200, {"content-type": mime.getType(path.basename(filePath))});
  response.end(fileContents);
}

var elevatorServer = require('./lib/elevatorserver');
elevatorServer.listen(server);
