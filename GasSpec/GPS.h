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
	void getData(long *tim, float *lat, float *lon, float *alt) ;
	void start() ;
	void stop() ;

	loc_t data ;
    	volatile float time, lat, lon, alt ;
	


} ;

	
