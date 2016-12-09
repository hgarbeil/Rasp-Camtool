#include "clientTCP.h"


clientTCP::clientTCP()  {

}
clientTCP::~clientTCP()  {
	close (sockfd) ;

}

void clientTCP::setHostInfo (const char *hostid, int portNum) {
	char tmess [240] ;
	int n ;

	sockfd = socket (AF_INET, SOCK_STREAM, 0) ;

	server = gethostbyname(hostid) ;
	if (server == NULL) {
		printf ("No such host : %s\r\n", hostid) ;
		exit(-1) ;
	}

	bzero ((char *)&serv_addr, sizeof (serv_addr)) ;
	serv_addr.sin_family = AF_INET ;
	bcopy ((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
		server->h_length) ;
	printf ("host name is %s\r\n", hostid) ;
	printf ("trying to connect on port %d\r\n", portNum) ;

	serv_addr.sin_port = htons (portNum) ;
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
		<0) {
		printf ("error connecting \r\n") ; ;
		exit(-1) ;
	}
	  
	strcpy (tmess, "yo there ") ;
	

}


int clientTCP::writeString (const char *mstring) {
	int n ;
	n = write (sockfd, mstring, strlen (mstring)) ;
	return (n) ;
}


/*
int main (int argc, char *argv[]) {
	
	clientTCP *ctcp = new clientTCP() ;
	ctcp->setHostInfo ("128.171.157.93", 8890) ;

}

	
*/
