// bbgServer.c
#include "bbgServer.h"
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

#define SOCKET_PORT 17433
#define MAX_RECEIVE_MESSAGE_LENGTH 1024
#define REPLY_BUFFER_SIZE (1500)
#define VALUES_PER_LINE 4

#define COMMAND_HELP        "help"
#define COMMAND_GET_ACCEL   "accel"
#define COMMAND_GET_ANGLE   "angle"
#define COMMAND_STOP        "stop"

static _Bool isShuttingDown = false;

static pthread_t s_threadId;
static char replyBuffer[REPLY_BUFFER_SIZE];

// Header
static void *listeningThread(void *args);
static void processCommand(char* command, int socketDescriptor, struct sockaddr_in *pSin);
//static int secondWordToInt(char *string);
//static void concatValuesToString(char *targetBuffer, int data[], int indexStart, int indexEnd);


void bbgServer_startListening(void)
{
	printf("bbg server started from bbgServer.c!\n");
	pthread_create(&s_threadId, NULL, &listeningThread, NULL);
}

void bbgServer_cleanup(void)
{
  isShuttingDown = true;
	pthread_join(s_threadId, NULL);
}


static void *listeningThread(void *args)
{

	// Buffer to hold packet data:
	char message[MAX_RECEIVE_MESSAGE_LENGTH];

	int socket_descriptor;
  int client_socket;
  socklen_t sout_len;

	// Address
	struct sockaddr_in s_in;
  struct sockaddr_in sout;

	s_in.sin_family = AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);	// Host to Network long
	s_in.sin_port = htons(SOCKET_PORT);				// Host to Network short

	// Create the socket
	socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	// Bind the socket to the port that we specify
	bind(socket_descriptor, (struct sockaddr*) &s_in, sizeof(s_in));
  // Listen for access requests
  listen(socket_descriptor, 10);


	while (!isShuttingDown) {
		// Empty buffer to make room for new message
		memset(message, 0, strlen(message));

		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		sout_len = sizeof(sout);
		//printf("%d, %p, %d\n", socket_descriptor, (void*) &sout, sout_len);
    client_socket = accept(socket_descriptor, (struct sockaddr *) &sout, &sout_len);

		int bytesRx;

		while((bytesRx = recv(client_socket, message, MAX_RECEIVE_MESSAGE_LENGTH, 0)) >0){

			// Make it null terminated (so string functions work):
			message[bytesRx] = 0;


			printf("Received message: %s\n", message);

			processCommand(message, socket_descriptor, &s_in);

			// Transmit a reply (if desired):
			if (strnlen(replyBuffer, REPLY_BUFFER_SIZE) > 0) {
				send(client_socket, replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE), 0);
			}
		}

		close(client_socket);
	}

	// Close socket on shut-down
	close(socket_descriptor);
	return NULL;
}

_Bool isThisCommand(char* message, const char* command)
{
	return strncmp(message, command, strlen(command)) == 0;
}


static void processCommand(char* command, int socketDescriptor, struct sockaddr_in *pSin)
{
	replyBuffer[0] = 0;

	// Extract the value from the message:
	if (isThisCommand(command, COMMAND_HELP)) {
	printf("recieved help request\n");
		sprintf(replyBuffer, "Accepted command examples:\n");
		strcat(replyBuffer, "accel -- Return raw accelerometer data.\n");
    strcat(replyBuffer, "angle  -- Return elevator tilt angle.\n");
		strcat(replyBuffer, "stop -- cause the server program to end.\n");
	}

	else if (isThisCommand(command, COMMAND_GET_ACCEL)) {
    accel_output *accel = AccelerationListener_getAccelOutput();
    sprintf(replyBuffer, "accel %d %d %d", accel->x, accel->y, accel->z);
    free(accel);
	}

  else if (isThisCommand(command, COMMAND_GET_ANGLE)) {
    sprintf(replyBuffer, "angle %lf", AccelerationListener_getAngle());
	}

	else if (isThisCommand(command, COMMAND_STOP)) {
		sprintf(replyBuffer, "Program terminating.\n");
		isShuttingDown = true;
	}

	else {
		sprintf(replyBuffer, "Unknown command. Type 'help' for command list.\n");
	}
}
