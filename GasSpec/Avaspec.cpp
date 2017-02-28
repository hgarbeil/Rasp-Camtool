#include "Avaspec.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <mutex>
#include <iostream>

using namespace std ;
std::mutex mtx ;

int Avaspec::intTimes [] = {50, 100, 250, 500, 750, 1000, 1500} ;
int Avaspec::nscansAvg [] = {8, 4, 4, 2, 2,1,1} ;
int Avaspec::maxDN = 16384. * .9 ;
bool Avaspec::darkReady[] = {false, false} ;
bool Avaspec::contReady[] = {false, false} ;
bool Avaspec::singleReady = false ;
bool Avaspec::autoReady = false ;
AvsHandle Avaspec::spec[] = {-1,-1} ;

void Avaspec::darkCallback (AvsHandle *av, int *result) {
        for (int i=0;i<2; i++){
		if (*av == spec[i]) {
			darkReady [i] = true  ;
			break ;
		}
	}
}

void Avaspec::dataCallback (AvsHandle *av, int *result) {
    singleReady = true ;
}

void Avaspec::contCallback (AvsHandle *av, int *result) {
   	int i ;
       for (i=0;i<2; i++) {
		if (*av == spec[i]) {
			contReady [i] = true  ;
			break ;
		}
	}
}

void Avaspec::autoCallback (AvsHandle *av, int *result) {
    autoReady = true ;
}

Avaspec::Avaspec() {
    	needsUpdate = new bool [2] ;
	needsUpdate[0] = true ;
    	needsUpdate[1] = true ;
	scansCollected [0] = 0 ;
	scansCollected [1] = 0 ;


	waves = 0l ;
	specData = 0L ;
    	dark = 0L ;

	npix = 3648 ;
    	nscansCollect = 500000 ;
    	nscansDark = 5 ;



    	autoReady = false ;
    	singleReady = false ;
	outdat = new float [3648*2] ;

    //timer = new QTimer () ;
    //connect (timer, SIGNAL (timeout()), this, SLOT (checkSpec())) ;
    //timer->start (20) ;
    	curLev = 1 ;
    	nspecs = 2 ;

}

int Avaspec::init() {
    int i ;
    unsigned int bset ;

    //printf ("Number of mini-avs specs found %n\r\n", nspecs) ;
    int n = AVS_Init(0) ;
    if (n<0) {
        cout <<"AVS Init failed- returned " << n  << endl ;
        return (-1);
    }
    nspecs = AVS_GetNrOfDevices() ;
	cout << "Number of devices is : " << nspecs << endl ;
    if (nspecs < 1) {
	cout << "Number of devices is : " << nspecs << endl ;
        return (-1) ;
    }
    n = AVS_GetList (sizeof(a_pList), &bset, &a_pList[0]) ;
    if (n < 1) {
	cout << "Could not get spec list " << endl ;
    }

    AVS_GetNumPixels(spec[0], &npix) ;
    double *dwaves = new double [npix * 2] ;
    for (i=0; i<nspecs; i++) 
    {
    	initMeasStruct (i) ;
    	spec[i] = AVS_Activate (&a_pList[i]) ;
    	AVS_GetLambda (spec[i], &dwaves[i*npix]) ;
	
    }

    // init the lambda arrays as well as the dark arrays
    waves = new float [npix * 2];
    dark = new double [npix * 2] ;
    specData = new double [npix * 2] ;
    for (i=0; i<nspecs; i++){ 
    for (int is=0;is<npix; is++){
            waves[i*npix+is] = (float) dwaves [i*npix+is] ;
            dark[i*npix+is]=0. ;
            specData[i*npix+is] = 0.;
    }


    AVS_SetPrescanMode(spec[i], true) ;
    }
    delete [] dwaves ;

    checkSpecRunning = true ;
    m_thread = std::thread (&Avaspec::checkSpec, this) ;
    m_thread.detach() ;



    return (1) ;

}


void Avaspec::start () {
    int i, status ;
    short nscans  ;

    nscans = 10 ;

	for (i=0; i<nspecs; i++) 
	{
		if (needsUpdate[i]) {
			status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
			needsUpdate[i] = false ;
		}
        AVS_StopMeasure (spec[i]) ;
        //status = AVS_Measure(spec[i], &Avaspec::dataCallback, nscans) ;
        status = AVS_Measure(spec[i], NULL, nscans) ;
	cout << "Measure status is " << status << endl ;


	}

}	

void Avaspec::stop () {
    for (int i=0; i<nspecs; i++)
    {
        AVS_StopMeasure (spec[i]) ;

    }

}


void Avaspec::setScanData (int nscans, double *d) {
    //this->outdat = d ;
    //this->nscansCollect = nscans ;
}

void Avaspec::initMeasStruct (int specNum){ 




	l_PrepareMeasData[specNum].m_StartPixel = 0 ;
    l_PrepareMeasData[specNum].m_StopPixel = 3647 ;
	l_PrepareMeasData[specNum].m_IntegrationTime = 100 ;
    l_PrepareMeasData[specNum].m_IntegrationDelay = 0 ;
    l_PrepareMeasData[specNum].m_NrAverages = 1 ;
    l_PrepareMeasData[specNum].m_CorDynDark.m_Enable = 0;
    l_PrepareMeasData[specNum].m_CorDynDark.m_ForgetPercentage = 0;
    l_PrepareMeasData[specNum].m_Smoothing.m_SmoothPix = 0;
    l_PrepareMeasData[specNum].m_Smoothing.m_SmoothModel = 0;
    l_PrepareMeasData[specNum].m_SaturationDetection = 0;
    l_PrepareMeasData[specNum].m_Trigger.m_Mode = 0;
    l_PrepareMeasData[specNum].m_Trigger.m_Source = 0;
    l_PrepareMeasData[specNum].m_Trigger.m_SourceType = 0;
    l_PrepareMeasData[specNum].m_Control.m_StrobeControl = 0;
    l_PrepareMeasData[specNum].m_Control.m_LaserDelay = 0;
    l_PrepareMeasData[specNum].m_Control.m_LaserWidth = 0;
    l_PrepareMeasData[specNum].m_Control.m_LaserWaveLength = 0;
    l_PrepareMeasData[specNum].m_Control.m_StoreToRam = 0;

}


void Avaspec::setPixels (int specnum, int start, int stop) {
	l_PrepareMeasData[specnum].m_StartPixel = start ;
	l_PrepareMeasData[specnum].m_StopPixel = stop ;

	AVS_PrepareMeasure (spec[specnum], &l_PrepareMeasData[specnum]) ; 
	needsUpdate[specnum] = true ;


}

void Avaspec::setIntegrationTime(int snum, float ftime) {
    l_PrepareMeasData[snum].m_IntegrationTime = ftime ;
    needsUpdate [snum] = true ;
}


void Avaspec::setIntegrationTime(int lev) {
    l_PrepareMeasData[0].m_IntegrationTime = Avaspec::intTimes[lev] ;
    l_PrepareMeasData[0].m_NrAverages = Avaspec::nscansAvg [lev] ;
    AVS_PrepareMeasure (spec[0], &l_PrepareMeasData[0]) ;
    needsUpdate[0] = false ;
    usleep (100000) ;
}

int Avaspec::autoIntegrate (int level) {

    int status = -1 ;
    l_PrepareMeasData[0].m_IntegrationTime = Avaspec::intTimes[level] ;
    l_PrepareMeasData[0].m_NrAverages = Avaspec::nscansAvg[level] ;
    while (status < 0) {
        status = AVS_PrepareMeasure(spec[0], &l_PrepareMeasData[0]) ;
	cout << "Preparing "<< endl ;
        usleep (200000) ;
    }
    status = -1 ;
    curLev = level ;
    //AVS_MeasureCallback (spec[0], &Avaspec::autoCallback, 1) ;

    while (status <0) {
	cout << " AI " << level << "  " << status << endl ;
        status = AVS_MeasureCallback(spec[0], &Avaspec::autoCallback, 1) ;
        if (status < 0) usleep (50000) ;
    }
    return (1) ;

}





Avaspec::~Avaspec() {
    int i ;
    for (i=0; i<nspecs; i++) AVS_Deactivate(spec[i]) ;
    //AVS_StopMeasure (spec[i]) ;
    //AVS_StopMeasure (spec[0]) ;
	AVS_Done() ;
    //delete [] spec ;
	delete [] specData ;
	delete [] waves ;
    delete [] dark ;

}

void Avaspec::takeDark() {
    int i,  is, specNum, count=0, ifail, isucc, status ;
    unsigned int timLabel ;
    short icols = 5 ;
    //this->goThread = true ;

    // init the dark array
    for (i=0; i<nspecs; i++) {
    	darkReady[i] = false ;
    	l_PrepareMeasData[i].m_NrAverages = 5 ;
    	status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
    	usleep (200000) ;
    	status = -1 ;
    	while (status <0) {
        	status = AVS_MeasureCallback(spec[i], &Avaspec::darkCallback, 1) ;
        	if (status < 0) usleep (50000) ;
	}
    	usleep (200000) ;
    }
}

void Avaspec::takeSingle() {
    int i,  is, specNum, count=0, ifail, isucc, status ;
    unsigned int timLabel ;
    short icols = 5 ;
    //this->goThread = true ;

    // init the dark array
    singleReady = false ;
    l_PrepareMeasData[0].m_NrAverages = 5 ;
    status = AVS_PrepareMeasure (spec[0], &l_PrepareMeasData[0]) ;
    usleep (200000) ;
    status = -1 ;
    while (status <0) {
        status = AVS_MeasureCallback(spec[0], &Avaspec::dataCallback, 1) ;
        if (status < 0) usleep (50000) ;
    }
}

void Avaspec::setContFlag (bool f) {
    contFlag = f ;
}

void Avaspec::takeCont() {
    int i,  is, specNum, count=0, ifail, isucc, status ;
    unsigned int timLabel ;
    short icols = 1 ;
    scansCollected [0] = scansCollected[1] = 0;
    //this->goThread = true ;

    // init the dark array
    for (i=0; i<nspecs; i++) {
    	contReady[i] = false ;
    	l_PrepareMeasData[i].m_NrAverages = 1 ;
    	status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
    	usleep (200000) ;
    	status = -1 ;
    	while (status <0) {
        	status = AVS_MeasureCallback(spec[i], &Avaspec::contCallback, 1) ;

	        if (status < 0) usleep (50000) ;
	}
    }
}

float Avaspec::getMax (float *dat) {
    int i ;
    float max = -1.E9 ;
    for (i=0;i<npix;i++) {
        if (dat[i]>max) {
            max = dat[i] ;
        }
    }
    return max ;
}

void Avaspec::checkSpec () {
    int i, status, count=0, is ;
    unsigned int timLabel ;
    //status = AVS_PollScan (spec[0]) ;

    while (checkSpecRunning) {

    for (i=0; i<nspecs; i++) 
    if (darkReady[i]) {

        cout  << "checkSpec : darkReady" << endl  ;
        mtx.lock() ;
        AVS_GetScopeData (spec[i], &timLabel, &specData[npix*i]) ;
        for (is=0; is<npix; is++) this->outdat[is] = specData[is] ;
        darkReady[i] = false ;
        for (is=0; is<npix; is++) dark[i*npix+is] = specData[is] ;
        mtx.unlock() ;
        l_PrepareMeasData[i].m_NrAverages = 1 ;
        AVS_PrepareMeasure(spec[i], &l_PrepareMeasData[i]) ;
    }

    if (singleReady) {

        cout << "checkSpec : singleReady" << endl ;
        AVS_GetScopeData (spec[0], &timLabel, specData) ;

        mtx.lock() ;
        for (is=0; is<npix; is++) this->outdat[is] = specData[is] - dark[is] ;
        mtx.unlock() ;
        singleReady = false ;
        //for (is=0; is<npix; is++) outdat[is] = specData[is] - dark[is];
        l_PrepareMeasData[0].m_NrAverages = 1 ;
        AVS_PrepareMeasure(spec[0], &l_PrepareMeasData[0]) ;
    }

    for (i=0; i<nspecs; i++) 
    if (contReady[i]) {
	cout << "Got data for spec : " << i << endl ;

        mtx.lock() ;
        AVS_GetScopeData (spec[i], &timLabel, specData) ;

        for (is=0; is<npix; is++) this->outdat[npix * i + is] = specData[is] - dark[npix *i +is] ;
        cout << "checkSpec : "  << i << scansCollected [i]++ << "  " << outdat[npix *i + 3000] << endl  ;
        contReady [i] = false ;
        mtx.unlock() ;
        //if (contFlag[i]) this->takeCont() ;
	scansCollected [i]++ ;
	if (scansCollected[i] >= 100) checkSpecRunning = false ;
	else {
		status = -1 ;
    		while (status <0) {
	        	status = AVS_MeasureCallback(spec[i], &Avaspec::contCallback, 1) ;
       		 	if (status < 0) usleep (50000) ;
			cout << "Next scan status : " << status << endl ;

		}
	}

    }

    if (autoReady){
        autoReady = false ;
        AVS_GetScopeData (spec[0], &timLabel, specData) ;

        mtx.lock() ;
        for (is=0; is<npix; is++) this->outdat[is] = specData[is]  ;
        mtx.unlock() ;
        usleep (100000) ;
        float mval = getMax (outdat) ;
        if (mval >= Avaspec::maxDN) {

        } else {
            if (curLev < 6){
                curLev++ ;
                autoIntegrate (curLev) ;
                cout << "checkSpec : autoReady : curlev : "<< curLev << endl  ;
		

            }
            else {

                setIntegrationTime(2);
                AVS_PrepareMeasure(spec[0], &l_PrepareMeasData[0]) ;
            }


        }

    }

	} // end of while
	cout << "Thread is closed " << endl ;
}

/*
 * int main() {


	Avaspec *aspec = new Avaspec() ;
	aspec->init() ;
	aspec->start() ;
	while (1) {
		usleep (1000000) ;

	}

} */
