#include <stdbool.h>

#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

void UDP_init();
void UDP_cleanup();

_Bool UDP_isShuttingDown();

#endif
