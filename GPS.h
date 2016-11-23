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
	void start() ;
	void stop() ;

	loc_t data ;
	volatile float lat, lon ;


} ;

	
