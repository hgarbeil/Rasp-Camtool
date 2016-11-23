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
	std::thread m_thread ;
	void newIMU() ;
	RTIMU *getIMU() {
		return m_imu ;
	}
}
;
