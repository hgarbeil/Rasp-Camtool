#include "GPS.h"
#include "IMUThread.h"
#include "Cam.h"



int main (int argc, char *argv[]) {
	
	int w, h ;
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
	imu->newIMU() ;
	gpst->start() ;
	imu->startThread () ;
	while (1) {
	}
}
