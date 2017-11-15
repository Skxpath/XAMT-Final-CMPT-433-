#ifndef _BBG_SERVER_H_
#define _BBG_SERVER_H_

// Start listening to the port on a separate thread.
void bbgServer_startListening(void);

void bbgServer_cleanup(void);

#endif
