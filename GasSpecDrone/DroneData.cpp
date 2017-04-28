#include "DroneData.h"

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
	*lat = bcd.pos.latitude ;
	*lon = bcd.pos.longitude ;
	*alt = bcd.pos.altitude ;

	*time = bcd.pos.time ;
}

	




