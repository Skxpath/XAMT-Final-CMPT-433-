// NOTE: main() is only for testing - delete when implementing into a bigger project

#include "accelerationListener.h"
#include "bbgServer.h"
#include "bbgClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	AccelerationListener_init();
  bbgServer_startListening();
  bbgClient_init();
  sleep(30);
	printf("finishing\n");
	long long sec = 2;
	//struct timespec thirtySeconds = {sec, 0};
	//nanosleep(&thirtySeconds, (struct timespec *) NULL);
	sleep(sec);
  bbgClient_cleanup();
  bbgServer_cleanup();
	AccelerationListener_cleanup();
	return 0;
}
