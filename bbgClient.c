#include "bbgClient.h"
#include "accelerationListener.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERV_PORT 8088
#define MAX_MESSAGE_LENGTH 1024
#define SERV_ADDR "192.168.7.1"

static _Bool stopFlag = false;

static pthread_t tid;
static void* bbgClient_thread(void *args);

void bbgClient_init(void){
  // Create thread
  pthread_create(&tid, NULL, bbgClient_thread, NULL);
}

void bbgClient_cleanup(void){
  // Set stopFlag to alert thread to stop
  stopFlag = true;

  // Wait for thread to stop
  pthread_join(tid, NULL);
}

static void* bbgClient_thread(void *args)
{
  // Declare variables to be used for socket construction
  char msg_out[MAX_MESSAGE_LENGTH];
  struct sockaddr_in servaddr;

  // Construct socket
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr= inet_addr(SERV_ADDR);
  servaddr.sin_port =  htons(SERV_PORT);

  // Repeatedly connect to server to update data while stopFlag is not set
  while(!stopFlag){

    // Connect to server & check if connection is successful
    if(connect(socket_desc, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("ERROR: Cannot connect to server.\n");
      sleep(10);
      continue;
    }

    // Reset message buffer
    memset(msg_out, 0, strlen(msg_out));

    // Prepare buffer to send current angle data
    sprintf(msg_out, "angle %lf", AccelerationListener_getAngle());

    // Send the angle data to the server
    send(socket_desc, msg_out, strlen(msg_out), 0);

    // Reset message buffer
    memset(msg_out, 0, strlen(msg_out));


    // Prepare accelerometer output
    accel_output *accel = AccelerationListener_getAccelOutput();

    // Save data to output buffer
    sprintf(msg_out, "accel %d %d %d", accel->x, accel->y, accel->z);

    // Send output buffer contents to server
    send(socket_desc, msg_out, strlen(msg_out), 0);

    // Free memory allocated by AccelerationListener_getAccelOutput();
    free(accel);

    // Close connection to server
    close(socket_desc);

    // Wait 1s before sending next update
    sleep(1);
   }

   // Terminate thread when stopFlag is set
   pthread_exit(0);
 }
