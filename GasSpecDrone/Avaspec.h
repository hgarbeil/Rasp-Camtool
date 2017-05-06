
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <thread>
#include "avaspec.h"
#include "phidgetsMot.h"
#include "GPS.h"
#include "IMUThread.h"
#include "DroneData.h"


using namespace std ;


class Avaspec  {
public :

	


	Avaspec() ;
	~Avaspec() ;
	
	int init() ;
	std::thread m_thread ;
    	AvsIdentityType a_pList[2] ;
	phidgetsMot *pm ;
	GPS *gps ;
	IMUThread *imu ;
	DroneData *droneData ;


    	static bool darkReady[2], singleReady, autoReady, contReady [2];
    	bool checkSpecRunning, autoScanning ;
	char workDir [420] ;

    	static AvsHandle spec [2];
    	float *waves ;
    	float *outdat, *dark ;
    	double *specData ; 


    	int curLev, curSpec, nspecs, nscansCollect, nscansDark ;
    	int scansCollected [2] ;
    	unsigned short npix ;

	void setPixels (int specnum, int start, int stop) ;
	void setPM(phidgetsMot *pm) ;
	void getFilePrefix (char *) ;
	void setGPS (GPS *) ;
	void setIMU (IMUThread *) ;
	void setDroneData (DroneData *) ;


    void initMeasStruct (int) ;
    void setScanData (int scans, double *dat) ;
    void setContFlag (bool f) ;
    void setIntegrationTime (int snum, float time) ;
    void setIntegrationTime (int snum, int lev) ;
    void checkSpec () ;
    void setWorkDir (const char *) ;
    int autoIntegrate (int spec, int lev) ;
    float getMax (float *dat) ;

    void takeCont() ;
    void takeSingle() ;
    void takeDark () ;
    void start() ;
    void stop () ;
    char *getTimeString (time_t) ;

    bool *needsUpdate, contFlag ;

    MeasConfigType l_PrepareMeasData[2];

    static void darkCallback (AvsHandle *hnd, int *result) ;
    static void dataCallback (AvsHandle *hnd, int *result) ;
    static void autoCallback (AvsHandle *hnd, int *result) ;
    static void contCallback (AvsHandle *hnd, int *result) ;

    static int intTimes [7], nscansAvg[7] ;
    static int maxDN ;
    // file units for output
    FILE *contUnit[2], *textUnit[2], *rawUnit[2] ;
    int lastMinute2 [2] ;

/*
signals :
    void gotData() ;
    void newInt (int) ;

public slots :
    void checkSpec() ;
*/

} ;

