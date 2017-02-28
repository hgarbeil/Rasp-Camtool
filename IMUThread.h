#ifndef IMU
#define IMU
#include "RTIMULib.h"
#include <thread>


class IMUThread 
{
public :	
	
	IMUThread() ;
	RTIMU *m_imu ;
	RTPressure *pressure ;
	RTIMUSettings *m_settings ;
	volatile float roll, pitch, yaw ;
	bool keepRunning ;
	int m_timer ;
	void startCollecting () ;
	void startThread () ;
	void getEuler (float *, float *, float *) ;
	std::thread m_thread ;
	void newIMU() ;
	RTIMU *getIMU() {
		return m_imu ;
	}
}
;

#endif 
