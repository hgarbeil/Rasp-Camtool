#include "GPS.h"
#include "mutex"

std::mutex mutlock ;

GPS::GPS () {

	this->init() ;
	keepRunning = true ;
    data.altitude = -10 ;
    data.latitude = 0.0 ;
    data.longitude = 0. ;
    lat = 0. ;
    lon = 0. ;
    alt = 0. ;
}

GPS::~GPS() {
    keepRunning = false ;

}

void GPS::init () {
	gps_init() ;
}

void GPS::start () {
	m_thread = std::thread (&GPS::startCollecting, this) ;
}

void GPS::stop () {
	keepRunning = false ;
}

void GPS::startCollecting () {
	

	while (keepRunning) {
        //mutlock.lock() ;
		gps_location (&data) ;
        //mutlock.unlock() ;
        //printf ("TIME : %ld  Lat : %lf\tLon : %lf\r\n", data.time,
        //	data.latitude, data.longitude) ;
        mutlock.lock() ;
		lat = data.latitude ;
		lon = data.longitude ;
        alt = data.altitude ;
        mutlock.unlock() ;
		
	}
}


		


	
