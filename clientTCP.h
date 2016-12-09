#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


class clientTCP {
public :

	clientTCP () ;
	~clientTCP () ;
	void setHostInfo (const char *, int portnum) ;
	int writeString (const char *mstring) ;

	struct sockaddr_in serv_addr ;
	struct hostent *server ;
	int sockfd, portnum ;


} ;
	

