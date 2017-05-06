#ifndef DData
#define DData

#include <string>
#include <cstring>
#include <unistd.h>

#include "LinuxSerialDevice.h"
#include "LinuxThread.h"
#include "LinuxSetup.h"
#include "LinuxCleanup.h"
#include "ReadUserConfig.h"

#include <DJI_Follow.h>
#include <DJI_Flight.h>
#include <DJI_Version.h>
#include <DJI_WayPoint.h>


using namespace std ;
using namespace DJI ;
using namespace DJI::onboardSDK ;


class DroneData {
public :
	
	DroneData () ;
	~DroneData () ;
	CoreAPI *api ;
	Flight *flight ;
	LinuxThread *readThread ;
	LinuxSerialDevice *serialDevice ;
	void getPositionData (long *time, float *lat, float *lon, float *alt) ;
	void getDateTime (int *yr, int *mon, int *day, int *hr, int *min, int *sec) ;
	void getTime (int *hr, int *min, int *sec) ;
	void getMinute (int *min) ;

} ;





#endif
