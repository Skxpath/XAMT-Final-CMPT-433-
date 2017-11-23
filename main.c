// NOTE: main() is only for testing - delete when implementing into a bigger project

#include "accelerationListener.h"
#include "bbgServer.h"
#include "bbgClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
<<<<<<< HEAD
  printf("bbg server started!");
  bbgServer_startListening();

	//AccelerationListener_init();
	//printf("finishing\n");
	//long long sec = 2;
	//struct timespec thirtySeconds = {sec, 0};
	//nanosleep(&thirtySeconds, (struct timespec *) NULL);
	//sleep(sec);
	//AccelerationListener_cleanup();

  sleep(500);
  bbgServer_cleanup();
  printf("bbg server terminated...");
=======
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
>>>>>>> 52cacacd6a11f51ff09a099387fab5ec86b8a06d
	return 0;
}
