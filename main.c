// NOTE: main() is only for testing - delete when implementing into a bigger project

#include "infoUpdater.h"
#include "udpListener.h"
#include "sleeping.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	Updater_init();
  UDP_init();
  while(!UDP_isShuttingDown()) {
    sleep_msec(300);
  }
	//struct timespec thirtySeconds = {sec, 0};
	//nanosleep(&thirtySeconds, (struct timespec *) NULL);
  UDP_cleanup();
	Updater_cleanup();
	return 0;
}
