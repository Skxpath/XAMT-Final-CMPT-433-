# Elevator Project by XATM - (Xinran) xza116, (Alex) cwzhang, (Tony) zifeiy, (Matt) mrudenko
## Iteration One 
### Things we decided that we need
* IR sensor to detect distance
* At least two Blutooth modules to send data from the BBG mounted on elevator 
* Bluetooth signal receivers (potentially phones?)
* 2 BBG boards with one acting as the sensing module and the other as a local server that connects to the internet and communicates with the central server

## Iteration Two
* We are using a wifi to communicate to the internet with the BBG, which talks to an external Node.JS server that can track and pay attention to the data in aggregate.
* On the BBG, we are measuring the status of the elevator with an accelerometer and infared sensor, which tells us the elevators tilt and floor the elevator is on. This information is collected and used.

## How to run the project 
make sure BBG is up and running.
*$cd (somewhere)/finalProject
*$make
*$node server.js
*#cd (somewhere)/public/myapp
*#./elevator  
