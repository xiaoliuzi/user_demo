#include "unp.h"

pid_t listenfd, connfd;
listenfd = Socket(...);

/* fill in sockaddr_in{} with server's well-known port    */

Bind(listenfd, ...);

Listen(listenfd, LISTENQ);

for ( ; ;) {

	connfd = Accept(listenfd, ... );      /* probably blocks */
	if ( (pid = Fork() ) == 0) {          /* return pid in parent progress, return 0 in child progress */
		Close(listenfd);                  /* child closes listening socket */
		doit(connfd);                     /* process the request */
		Close(connfd);                    /* done with this client */
		exit(0);                          /* children terminates */
	}
	
	Close(connfd);                        /* parent closes connected socket */
}
