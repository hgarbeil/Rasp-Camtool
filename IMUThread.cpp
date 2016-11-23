#include "IMUThread.h"
#include "GPS.h"

IMUThread::IMUThread() 
{
	m_imu = NULL ;
	m_settings = new RTIMUSettings ("RTIMULib") ;
	m_timer = -1 ;
	keepRunning = true ;
	roll = 0. ;
	pitch = 0. ;
	yaw = 0. ;
}

void IMUThread::newIMU() 
{
	if (m_imu != NULL) {
		delete m_imu ;
		m_imu = NULL ;
	}
	m_imu = RTIMU::createIMU (m_settings) ;
	m_imu->IMUInit() ;
	m_imu->setGyroEnable(true) ;
	m_imu->setAccelEnable (true) ;
	m_imu->setCompassEnable(true) ;
	pressure = RTPressure::createPressure(m_settings) ;
	pressure->pressureInit() ;
	
	
	
	if (m_timer != -1) {
		//killTimer (m_timer) ;
		m_timer = -1 ;
	}

}

void IMUThread::startThread () {
	m_thread = std::thread (&IMUThread::startCollecting, this) ; 
}


void IMUThread::startCollecting() {
	RTIMU_DATA rdata ;
	
while (1) {
	usleep (m_imu->IMUGetPollInterval() * 10000) ;
	while (m_imu->IMURead() && keepRunning) {
		printf ("In the loop\r\n") ;
		rdata = m_imu->getIMUData() ;
		pressure->pressureRead(rdata) ;
		roll = rdata.fusionPose.x() ;
		pitch = rdata.fusionPose.y() ;
		yaw = rdata.fusionPose.z() ;
		//rdata = m_imu->getCompass() ;
		
		
		//printf ("Temp : %f  Press : %f Alt : %f\r\n", rdata.temperature, rdata.pressure, RTMath::convertPressureToHeight(rdata.pressure) ) ;
		//printf ("%f %f %f\r\n", rdata.compass.x(), rdata.compass.y(), rdata.compass.z()) ;
		//printf ("%s\r\n\n",RTMath::displayDegrees("",rdata.fusionPose)) ;
		//printf ("%f %f %f\r\n", rdata.fusionPose.x(), rdata.fusionPose.y(),
	//		rdata.fusionPose.z()) ;

	}
//	printf ("nother read\r\n") ;
}	
}


int main (int argc, char *argv[]) {
	
	IMUThread *imu = new IMUThread() ;
	GPS *gpst = new GPS () ;
	imu->newIMU() ;
	gpst->start() ;
	imu->startThread () ;
	while (1) {
	}
}
