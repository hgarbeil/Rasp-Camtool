#ifndef GPSD
#define GPSD 

extern "C" {
#include <gps.h>
}
#include <stdio.h>
#include <thread>




class GPS {

public :
	GPS () ;
	~GPS () ;

	bool keepRunning ;
	std::thread m_thread ;
	void init () ;
	void startCollecting() ;
	void getData(long *date, long *tim, float *lat, float *lon, float *alt) ;
	void getDateString(int *day, int *mon, int *yr, long *tim) ;
	void start() ;
	void stop() ;

	loc_t data ;
    	volatile float  lat, lon, alt ;
	volatile long date, time ;
	//volatile int day_month, month, year ;
	


} ;

#endif	
