#include "GPS.h"
#include "IMUThread.h"
#include "Cam.h"
#include "clientTCP.h"



int main (int argc, char *argv[]) {
	
	int w, h, count =0 ;
	char outstring [240] ;
	unsigned char *data ;
	CamC c ;
	c.camopen () ;
	c.camparams() ;
	w = c.w ;
	h = c.h ;
	data = new unsigned char [w * h * 3] ;
	c.getImage (data) ;
	c.writeJPeg ("test.jpg", data) ;
	
	delete [] data ;
	IMUThread *imu = new IMUThread() ;
	GPS *gpst = new GPS () ;
	clientTCP *ctcp = new clientTCP () ;
	ctcp->setHostInfo ("128.171.157.93", 8890) ;
	imu->newIMU() ;
	gpst->start() ;
	imu->startThread () ;
	while (1) {
		sprintf (outstring, "outstring %d\r\n", count++) ;
		ctcp->writeString (outstring) ;
		usleep (1000000) ;
		
	}
}
