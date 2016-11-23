#include "GPS.h"


GPS::GPS () {

	this->init() ;
	keepRunning = true ;
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
		gps_location (&data) ;
		printf ("TIME : %ld  Lat : %lf\tLon : %lf\r\n", data.time,
			data.latitude, data.longitude) ;
		lat = data.latitude ;
		lon = data.longitude ;
		
	}
}


		


	
