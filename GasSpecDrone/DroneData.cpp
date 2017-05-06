#include "DroneData.h"
#include <math.h>

#define R2D 180./M_PI

DroneData::DroneData() {
	
	unsigned short broadcastAck ;
	readUserConfig() ;
	serialDevice = new LinuxSerialDevice (UserConfig::deviceName, UserConfig::baudRate) ;	
	api = new CoreAPI (serialDevice) ;
	flight = new Flight (api) ;
	cout << "Start the linux thread " << endl ;

	// start the read thread to poll the drone's serial port for position and attitude
	readThread = new LinuxThread (api, 2) ;
	
	// actually start the thread
	int setupStatus = setup (serialDevice, api, readThread) ;
 
	broadcastAck = api->setBroadcastFreqDefaults(1) ;
	api->getDroneVersion (1) ;
	usleep (100000) ;

	
	
} 

DroneData::~DroneData() {
	int cleanupStatus = cleanup (serialDevice, api, flight, readThread) ;
}

void DroneData::getPositionData (long *time, float *lat, float *lon, float *alt) {
	
	BroadcastData bcd ;

	bcd = api->getBroadcastData () ;
	*lat = bcd.pos.latitude *R2D ;
	*lon = bcd.pos.longitude *R2D ;
	*alt = bcd.pos.altitude ;

	//*time = bcd.pos.time ;
}

	
void DroneData::getDateTime (int *yr, int *mon, int *day, int *hr, int *min, int *sec) {
	time_t current_time ;
	struct tm *ts ;

	time(&current_time) ;
	ts = localtime (&current_time) ;

	*year = ts->tm_year + 1900 ;
	*mon = ts->tm_mon + 1 ;
	*day = ts->tm_mday ;
	*hr = ts->tm_hour  ;
	*min = ts->tm_min ;
	*sec = ts->tm_sec ;
}
	
void DroneData::getTime ( int *hr, int *min, int *sec) {
	time_t current_time ;
	struct tm *ts ;

	time(&current_time) ;
	ts = localtime (&current_time) ;
	*hr = ts->tm_hour  ;
	*min = ts->tm_min ;
	*sec = ts->tm_sec ;

}

	



