#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "Avaspec.h"
#include "GPIO.h"

int main(int argc, char *argv[])
{
	int status , counter = 0 ;

	Avaspec *avs = new Avaspec () ;
	GPIO *gp = new GPIO() ;
	gp->init() ;
	status = avs->init() ;

	printf ("avs init status is %d\n", status) ;

	gp->toggle(1) ;
	avs->takeDark() ;
	usleep (500000) ;
	gp->toggle(0) ;

	avs->takeCont() ; 
	
	usleep (15000000) ;

/*
	while (avs->checkSpecRunning) {
	
	
		usleep (100000) ;
		counter++ ;
	}
*/

	gp->toggle(0) ;
	usleep (100000) ;
	delete avs ;
	cout << "delete the gp object " << endl ;
	delete gp ;


}
