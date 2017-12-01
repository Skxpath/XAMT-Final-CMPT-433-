#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#include "infoUpdater.h"
#include "sleeping.h"

static _Bool shuttingDown = false;

static pthread_t tid;
static void* udp_pthread(void *ar);

void UDP_init()
{
  pthread_create(&tid, NULL, &udp_pthread, NULL);
}

static void* udp_pthread(void *ar)
{
  struct sockaddr_in sin;
  int sock;
  int success;

  const int BBG_PORT = 12345;
  // initialize the socket
  sock = socket(PF_INET, SOCK_DGRAM, 0);
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(BBG_PORT);

  //checks if socket is successfully binded or not
  success = bind(sock, (struct sockaddr*) &sin, sizeof(sin));

  if(success < 0){
    printf("Server initialization failed\n");
  } else {
    printf("Server initialization success\n");
  }


  char* token; //for tokenizing the input
  char saveptrVal = '0';
  char *saveptr = &saveptrVal; //for strtok_r function - taken from the man page
  char* args[10]; //for commands
  int argsSize; //self-explanatory

  do {
    static char message[1024];
    static char answer[1024];
    unsigned int sinLen = sizeof(sin);
    argsSize = 0;
    recvfrom(sock, message, 1024, 0, (struct sockaddr*) &sin, &sinLen);

    // Remove carriage return from the end of user input.
    // Source of the idea: http://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    char* retCh;
    if ((retCh = strchr(message, '\n')) != NULL)
      *retCh = '\0';

    printf("Recieved: ");
    printf("%s\n", message);

    //tokenize the nput into separate commands
    token = strtok_r(message, " ", &saveptr);
    int i;

    for (i = 0; token != NULL; i++)
    {
      args[i] = strdup(token);
      argsSize++;
      token = strtok_r(NULL, " ", &saveptr);
    }
    args[i] = NULL; //precaution

    if (args[0] == NULL)
    {
      strcpy(answer, "Empty request\n");
    }
    else if (strcmp(args[0], "accelerating") == 0)
    {
      if(Updater_isAccelerating()){
        strcpy(answer, "accelerating true");
      }else{
        strcpy(answer, "accelerating false");
      }
    }

    else if (strcmp(args[0], "angle") == 0)
    {
      strcpy(answer, "");
      sprintf(answer, "angle %lf", Updater_getAngle());
    }
    else if (strcmp(args[0], "accel") == 0)
    {
      strcpy(answer, "");
      sprintf(answer, "accel %lf %lf %lf", Updater_getAccelX(), Updater_getAccelY(), Updater_getAccelZ());
    }

    else if (strcmp(args[0], "totalaccel") == 0){
      strcpy(answer, "");
      sprintf(answer, "totalaccel %lf", Updater_getAccelTotal());
    }

    else if (strcmp(args[0], "distance") == 0){
      strcpy(answer, "");
      sprintf(answer, "distance %lf", Updater_getDistanceRaw());
    }

    else if (strcmp(args[0], "stabledist") == 0){
      strcpy(answer, "");
      sprintf(answer, "stabledist %lf", Updater_getDistanceStable());
    }

    else if(strcmp(args[0], "stop") == 0){
      printf("Server terminating\n");
      strcpy(answer, "Stopping server.\n");
      shuttingDown = true;
    }
    else
    {
      strcpy(answer, "Invalid command\n");
    }

    sinLen = sizeof(sin);
    printf("Sending: %s\n", answer);
    sendto(sock, answer, strlen(answer), 0,(struct sockaddr*) &sin, sinLen);
    for(int j = 0; j < i; j++) {
      free(args[j]);
    }
  } while (!shuttingDown);

  close(sock);
  return NULL;
}

void UDP_cleanup()
{
  shuttingDown = true;
  pthread_join(tid, NULL);
}

_Bool UDP_isShuttingDown()
{
  return shuttingDown;
}
