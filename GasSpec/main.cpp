// command line program to retrieve spectrometer data, integrates GPS, imu
// and temp, humidity info

#include <unistd.h>
#include <iostream>
#include "Avaspec.h"
#include "GPIO.h"
#include "GPS.h"
#include "phidgetsMot.h"

int main(int argc, char *argv[])
{
	float lat, lon, alt ;
	long gtime ;
	int status , counter = 0 ;

	// declare the spectrometer, motor and gps classes
	Avaspec *avs = new Avaspec () ;
	phidgetsMot *pm = new phidgetsMot() ;
	GPS *gps = new GPS() ;
	gps->init() ;
	gps->start() ;

	// get the start gps info
	sleep (5) ;
	gps->getData (&gtime, &lat, &lon, &alt) ;
	cout << "time : "<<gtime  << endl ;
	cout << "lat : "<< lat  << endl ;
	cout << "lon : "<< lon  << endl ;
	cout << "alt : "<< alt  << endl ;

	cout << "Position dark " << endl ;
	avs->setPM (pm) ;
 	avs->setWorkDir ("/home/pi/data") ;

	//GPIO *gp = new GPIO() ;
	//gp->init() ;

	status = avs->init() ;
	cout <<"Setting ref/cleark " << endl ;
	usleep (100000) ;
	pm->setRef() ;
	usleep (100000) ;


	avs->autoScanning = true ;
	avs->autoIntegrate(0, 0) ;
	while (avs->autoScanning) {
		usleep(10000) ;
	}
	avs->autoIntegrate(1, 0) ;
	while (avs->autoScanning) {
		usleep(10000) ;
	}

	printf ("avs init status is %d\n", status) ;

	//gp->toggle(1) ;
	pm->setDark() ;
	usleep (100000) ;
	avs->takeDark() ;
	usleep (500000) ;
	//gp->toggle(0) ;

	cout << "Take dark " << endl   ;
	avs->takeDark() ;
	usleep (500000) ;
	cout << endl<<"***********"<< endl<< "Start Scanning " << endl   ;



	pm->setRef() ;
	usleep (500000) ;
	avs->takeCont() ; 
	
	//usleep (15000000) ;


	while (avs->checkSpecRunning) {
	
	
		usleep (100000) ;
		counter++ ;
	}

	//gp->toggle(0) ;
	usleep (100000) ;
	delete avs ;
	cout << "delete the gp object " << endl ;
	//delete gp ;


}
