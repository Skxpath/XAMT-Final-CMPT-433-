// NOTE: main() is only for testing - delete when implementing into a bigger project

#include "accelerationListener.h"
#include "bbgServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  bbgServer_startListening();
	AccelerationListener_init();
	printf("finishing\n");
	long long sec = 2;
	//struct timespec thirtySeconds = {sec, 0};
	//nanosleep(&thirtySeconds, (struct timespec *) NULL);
	sleep(sec);
	AccelerationListener_cleanup();
  bbgServer_cleanup();
	return 0;
}
