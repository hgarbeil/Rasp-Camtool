#include <stdio.h>
#include <sys/types.h>
#include <thread>
#include "avaspec.h"
#include <QObject>
#include <QTimer>

using namespace std ;


class Avaspec : public QObject {
    Q_OBJECT
public :



	Avaspec() ;
	~Avaspec() ;
	
	int init() ;
	//void getSpectrometer (int specnum) ;
    AvsIdentityType a_pList[2] ;
    static bool darkReady, singleReady, autoReady, contReady ;
    AvsHandle spec [2];
    float *waves ;
    double *outdat ;
    double *specData, *dark ;

    int curLev, nspecs, nscansCollect, nscansDark ;
    unsigned short npix ;

	void setPixels (int specnum, int start, int stop) ;


	void initMeasStruct (int) ;
    void setScanData (int scans, double *dat) ;
    void setContFlag (bool f) ;
    void setIntegrationTime (int snum, float time) ;
    void setIntegrationTime (int lev) ;
    int autoIntegrate (int lev) ;
    float getMax (double *dat) ;

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
    QTimer *timer ;

signals :
    void gotData() ;
    void newInt (int) ;

public slots :
    void checkSpec() ;

} ;

