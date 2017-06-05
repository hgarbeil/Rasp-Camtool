// command line program to retrieve spectrometer data, integrates GPS, imu
// and temp, humidity info

#include <unistd.h>
#include <iostream>
#include "Avaspec.h"
#include "GPIO.h"
#include "GPS.h"
#include "IMUThread.h"
#include "phidgetsMot.h"

int main(int argc, char *argv[])
{
	float lat, lon, alt ;
	long gtime, gdate ;
	int status , counter = 0 ;
	int day, mon, year ;

	// declare the spectrometer, motor and gps classes
	sleep (2) ;
	//cout << "starting spec class " << endl ;
	Avaspec *avs = new Avaspec () ;
	//cout << "starting phidg class " << endl ;
	phidgetsMot *pm = new phidgetsMot() ;
	//cout << "starting gps class " << endl ;
	GPS *gps = new GPS() ;
	gps->init() ;
	gps->start() ;

	//cout << "starting imu lass " << endl ;
	IMUThread *imu = new IMUThread () ;
	//cout << "createdg imu lass " << endl ;
	imu->newIMU() ;
	imu->startThread() ;
	sleep (5) ;
	

	// get the start gps info
	cout << "GET GPS 5" << endl ;
	gps->getData (&gdate, &gtime, &lat, &lon, &alt) ;
	cout <<  "got first gps" << endl ;
	cout << "date : "<<gdate  << endl ;
	cout << "time : "<<gtime  << endl ;
	cout << "lat : "<< lat  << endl ;
	cout << "lon : "<< lon  << endl ;
	cout << "alt : "<< alt  << endl ;
	gps->getDateString (&day, &mon, &year, &gtime) ;

	cout << "Position dark " << endl ;
	avs->setPM (pm) ;
 	avs->setWorkDir ("/specdata") ;
	avs->setGPS (gps) ;
	avs->setIMU (imu) ; 

	//GPIO *gp = new GPIO() ;
	//gp->init() ;

	status = avs->init() ;
	//cout <<"Sleeping" << endl ;
	usleep (100000) ;
	cout <<"Setting ref/clear" << endl ;
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
