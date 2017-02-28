
#include <sys/types.h>
#include <thread>
#include "avaspec.h"

using namespace std ;


class Avaspec  {
public :

	


	Avaspec() ;
	~Avaspec() ;
	
	int init() ;
	std::thread m_thread ;
    	AvsIdentityType a_pList[2] ;
    	static bool darkReady[2], singleReady, autoReady, contReady [2];
    	bool checkSpecRunning ;

    	static AvsHandle spec [2];
    	float *waves ;
    	float *outdat ;
    	double *specData, *dark ;

    	int curLev, nspecs, nscansCollect, nscansDark ;
    	int scansCollected [2] ;
    	unsigned short npix ;

	void setPixels (int specnum, int start, int stop) ;


    void initMeasStruct (int) ;
    void setScanData (int scans, double *dat) ;
    void setContFlag (bool f) ;
    void setIntegrationTime (int snum, float time) ;
    void setIntegrationTime (int lev) ;
    void checkSpec () ;
    int autoIntegrate (int lev) ;
    float getMax (float *dat) ;

    void takeCont() ;
    void takeSingle() ;
    void takeDark () ;
    void start() ;
    void stop () ;

    bool *needsUpdate, contFlag ;
    MeasConfigType l_PrepareMeasData[2];

    static void darkCallback (AvsHandle *hnd, int *result) ;
    static void dataCallback (AvsHandle *hnd, int *result) ;
    static void autoCallback (AvsHandle *hnd, int *result) ;
    static void contCallback (AvsHandle *hnd, int *result) ;

    static int intTimes [7], nscansAvg[7] ;
    static int maxDN ;

/*
signals :
    void gotData() ;
    void newInt (int) ;

public slots :
    void checkSpec() ;
*/

} ;

