#include "GPS.h"
#include "mutex"

std::mutex mutlock ;

GPS::GPS () {

    this->init() ;
    keepRunning = true ;
    data.time = 0 ;
    data.date = 0 ;
    data.altitude = -10 ;
    data.latitude = 0.0 ;
    data.longitude = 0. ;
    
    lat = 0. ;
    lon = 0. ;
    alt = 0. ;
    hr = 0 ;
    min = 0 ;
    sec = 0 ;
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

void GPS::getData(long *dat, long *tim, float *lat, float *lon, float *alt) {
	
	*dat = data.date ;
	*tim = data.time ;
	*lat = data.latitude ;
	*lon = data.longitude ;
	*alt = data.altitude ;

	

	return ;
}
void GPS::stop () {
	keepRunning = false ;
}

void GPS::startCollecting () {
	

	while (keepRunning) {
        mutlock.lock() ;
	gps_location (&data) ;
        //mutlock.unlock() ;
        //printf ("TIME : %ld  Lat : %lf\tLon : %lf Alt: %lf\r\n", data.time,
        //	data.latitude, data.longitude, data.altitude) ;
        //mutlock.lock() ;
	lat = data.latitude ;
	lon = data.longitude ;
	date = data.date ;
        alt = data.altitude ;
	time = data.time ;
	hr = time / 10000 ;
	min = (time - (hr * 10000)) / 100 ;
	sec = time - (hr * 10000) - (min * 100) ;
	
        mutlock.unlock() ;
		
	}
}


void GPS::getDateString (int *day_of_month, int *mon, int *yr, long *tim) {
	
	*day_of_month = data.date / 10000 ;
	*mon = (data.date - (*day_of_month * 10000 )) / 100 ;
	*yr = (data.date - (*day_of_month * 10000 )) - (100 * *mon) + 2000 ; 
	*tim = data.time ;

}
	
