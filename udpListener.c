#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>


static pthread_t tid;
static void* udp_pthread(void *args);

void UDP_init()
{
  pthread_create(&tid, NULL, &udp_pthread, NULL);
}

static void* udp_pthread(void *args)
{
  struct sockaddr_in sin;
  int sock;

  // initialize the socket
  sock = socket(PF_INET, SOCK_DGRAM, 0);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(12345);
  bind(sock, (struct sockaddr*) &sin, sizeof(sin));

  int keepListening = 1;
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

    // really long if-else statement for provessing messages
    // ************************************************







    // **********************************************

    sinLen = sizeof(sin);
    sendto(sock, answer, strlen(answer), 0,(struct sockaddr*) &sin, sinLen);
    for(int j = 0; j < i; j++) {
      free(args[j]);
    }
  } while (keepListening == 1);

  close(sock);
  return NULL;
}

void UDP_cleanup()
{
  pthread_join(udp_pthread, NULL);
}
