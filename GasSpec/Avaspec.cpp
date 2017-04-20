#include "Avaspec.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <iostream>
#include <time.h>

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
	darkReady[0] = false ;
	darkReady[1] = false ;
        for (int i=0;i<2; i++){
		if (*av == spec[i]) {
			darkReady [i] = true  ;
			break ;
		}
	}
}

void Avaspec::setGPS (GPS *g) {
	gps = g ;
}

void Avaspec::setIMU (IMUThread *im) {
	imu = im ;
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

void Avaspec::setPM (phidgetsMot *Pm) {
	pm = Pm ;
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
    	nscansCollect = 800 ;
    	nscansDark = 5 ;
    	autoReady = false ;
    	singleReady = false ;
	outdat = new float [npix*2] ;
    	curLev = 1 ;
    	nspecs = 2 ;
	lastMinute2[0] = -1 ;
	lastMinute2[1] = -1 ;


}

int Avaspec::init() {
    int i, ispec ;
    char wavefile [420], prefix[420] ;
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
	getFilePrefix (prefix) ;
	sprintf (wavefile, "%s/Wave_%s_%01d.txt", workDir, prefix, i) ;
	FILE *fout = fopen (wavefile, "w") ;
    	spec[i] = AVS_Activate (&a_pList[i]) ;
    	AVS_GetLambda (spec[i], &dwaves[i*npix]) ;
    	for (int is=0;is<npix; is++){
		fprintf (fout, "%d\t%lf\r\n", is, dwaves[i*npix+is]) ;
	}
	fclose (fout) ;
	
    }

    // init the lambda arrays as well as the dark arrays
    waves = new float [npix * 2];
    dark = new float [npix * 2] ;
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


void Avaspec::setIntegrationTime(int specnum, int lev) {
    l_PrepareMeasData[specnum].m_IntegrationTime = Avaspec::intTimes[lev] ;
    l_PrepareMeasData[specnum].m_NrAverages = Avaspec::nscansAvg [lev] ;
    AVS_PrepareMeasure (spec[specnum], &l_PrepareMeasData[specnum]) ;
    needsUpdate[specnum] = false ;
    usleep (100000) ;
}

int Avaspec::autoIntegrate (int specnum, int level) {

    int status = -1 ;
    autoScanning = true ;
    curSpec = specnum ;
    l_PrepareMeasData[specnum].m_IntegrationTime = Avaspec::intTimes[level] ;
    l_PrepareMeasData[specnum].m_NrAverages = Avaspec::nscansAvg[level] ;
    while (status < 0) {
        status = AVS_PrepareMeasure(spec[specnum], &l_PrepareMeasData[specnum]) ;
	//cout << "Preparing "<< endl ;
        usleep (200000) ;
    }
    status = -1 ;
    curLev = level ;
    //AVS_MeasureCallback (spec[0], &Avaspec::autoCallback, 1) ;

    while (status <0) {
	cout << " AI " << level << "  " << status << endl ;
        status = AVS_MeasureCallback(spec[specnum], &Avaspec::autoCallback, 1) ;
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

// takeSingle not being used here.. used for debugging etc
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
    char fname [240], prefix [240] ;
    int i,  is, specNum, count=0, ifail, isucc, status ;
    unsigned int timLabel ;
    short icols = 1 ;
    scansCollected [0] = scansCollected[1] = 0;
    time_t cur_time ;

    // init the dark array

    
    // position the filter wheel in start scanning
    pm->setRef() ;
    for (i=0; i<nspecs; i++) {
	cur_time = time(NULL) ;
	//sprintf (fname, "%s/Scan_%s_%01d.bin", workDir, getTimeString (cur_time), i) ; 
	getFilePrefix (prefix) ;
	sprintf (fname, "%s/Scant_%s_%01d.bin", workDir, prefix, i) ; 
	contUnit[i] = fopen (fname, "w") ;
	lastMinute2[i] = gps->min/2 ;


    	contReady[i] = false ;
    	l_PrepareMeasData[i].m_NrAverages = 1 ;
    	status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
    	usleep (200000) ;
    	status = -1 ;
    	while (status <0) {
        	status = AVS_MeasureCallback(spec[i], &Avaspec::contCallback, nscansCollect) ;

	        if (status < 0) usleep (50000) ;
	}
		
    }
}

// returns the max value from the input float spectrum
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

// gets data from spectrometer and does whatever is appropriate
void Avaspec::checkSpec () {
    int i, status, count=0, is, curMin2 ;
    int curTime2 ;
    float scanNum ;
    unsigned int timLabel ;
    time_t cur_time ;
    char *c_time_string ;
    char fname [420], prefix[420], txtfile[420] ;
    //status = AVS_PollScan (spec[0]) ;

    while (checkSpecRunning) {

    for (i=0; i<nspecs; i++) 
    if (darkReady[i]) {
	// get current time for file naming
	cur_time = time(NULL) ;
	c_time_string = ctime (&cur_time) ;
	getFilePrefix (prefix) ;
	sprintf (fname, "%s/Dark_%s_%01d.bin", workDir, prefix, i) ; 
	FILE *fin = fopen (fname, "w") ;
	

        cout  << "checkSpec : darkReady" << endl  ;
	cout << "Writing : "<< fname << endl ;
        mtx.lock() ;
        AVS_GetScopeData (spec[i], &timLabel, &specData[npix*i]) ;
        //for (is=0; is<npix; is++) this->outdat[is] = specData[i*npix+is] ;
        darkReady[i] = false ;
        for (is=0; is<npix; is++) dark[i*npix+is] = specData[i*npix+is] ;
        mtx.unlock() ;
	fwrite ((char *) &dark [i * npix], 4, npix, fin) ;
	fclose (fin) ;
        l_PrepareMeasData[i].m_NrAverages = 1 ;
        AVS_PrepareMeasure(spec[i], &l_PrepareMeasData[i]) ;
    }

    if (singleReady) {

        cout << "checkSpec : singleReady" << endl ;
        AVS_GetScopeData (spec[0], &timLabel, specData) ;

        mtx.lock() ;
        //for (is=0; is<npix; is++) this->outdat[is] = specData[is] - dark[is] ;
        mtx.unlock() ;
        singleReady = false ;
        //for (is=0; is<npix; is++) outdat[is] = specData[is] - dark[is];
        l_PrepareMeasData[0].m_NrAverages = 1 ;
        AVS_PrepareMeasure(spec[0], &l_PrepareMeasData[0]) ;
    }

    for (i=0; i<nspecs; i++) 
    if (contReady[i]) {

	curTime2 = gps->min / 2 ;
	if (curTime2 != lastMinute2[i]) {
		getFilePrefix(prefix) ;
		lastMinute2[i] = curTime2 ;
		sprintf (fname, "%s/Cont_%s_%01d.bin", workDir, prefix, i) ; 
		fclose (contUnit[i]) ;
		contUnit[i]=fopen (fname, "w") ;
	}

        mtx.lock() ;
        AVS_GetScopeData (spec[i], &timLabel, &specData[i*npix]) ;
	scanNum = (float) scansCollected[i] ; 
	fwrite ((char *)&scanNum, 4, 1, contUnit[i]) ; 
        for (is=0; is<npix; is++) this->outdat[npix * i + is] = specData[i*npix+is] - dark[npix *i +is] ;
	fwrite ((char *)(&outdat[npix*i]), 4, 3648, contUnit[i]) ;
	cout << "write spec scan  : " << i << "  " << 
		scansCollected[i] << endl ;
	scansCollected[i]++ ;;  
        contReady [i] = false ;
        mtx.unlock() ;
        //if (contFlag[i]) this->takeCont() ;
	/*
	else {
		status = -1 ;
    		while (status <0) {
	        	status = AVS_MeasureCallback(spec[i], &Avaspec::contCallback, 1) ;
       		 	if (status < 0) usleep (50000) ;
			cout << "Next scan status : " << status << endl ;

		}
	}
	*/
	if (scansCollected[0] >= nscansCollect && scansCollected[1] >= nscansCollect) checkSpecRunning = false ;

    }

    if (autoReady){
        autoReady = false ;
        AVS_GetScopeData (spec[curSpec], &timLabel, specData) ;

        mtx.lock() ;
        for (is=0; is<npix; is++) this->outdat[is] = specData[is]  ;
        mtx.unlock() ;
        usleep (100000) ;
        float mval = getMax (outdat) ;
	cout << intTimes[curLev] << " : Max val is " <<mval << endl ;
	
        if (mval >= Avaspec::maxDN ) {
		cout << "Integration time to be set at " << intTimes[curLev] << endl ;
		autoScanning = false ;
	
	

        } else {
	    // if max dn not reached and not the max int time, increase
	    // int time
            if (curLev < 6){
                curLev++ ;
		autoScanning = true ;
                autoIntegrate (curSpec, curLev) ;
		
                cout << "checkSpec : autoReady : checking at  "<< intTimes [curLev]<< endl  ;
		

            }
            else {
                // max dn not reached but max int time, wrap it up
                cout << "Max never reached" << endl ;
                cout << "Int time set at" << intTimes [6]  << endl  ;
                setIntegrationTime(curSpec, 6);
                AVS_PrepareMeasure(spec[curSpec], &l_PrepareMeasData[curSpec]) ;
                autoScanning = false ;
                cout << "AutoScanning turned off" << endl  ;
            }
	}



    }

	} // end of while
	cout << "Thread is closed " << endl ;
	fclose (contUnit[0]) ;
	fclose (contUnit[1]) ;

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


void Avaspec::setWorkDir (const char *dir) {

	strcpy (workDir, dir);
}


char *Avaspec::getTimeString (time_t intime) {
	char *outstring = new char [120] ;
	struct tm *ltime = localtime (&intime) ;
	sprintf (outstring, "%03d%02d%02d_%02d%02d%02d", ltime->tm_year,ltime->tm_mon, ltime->tm_mday,
		ltime->tm_hour, ltime->tm_min, ltime->tm_sec) ;
	return outstring ;
}
	

void Avaspec::getFilePrefix (char *prefx) {
	
	int yr, mon, day ;
	long gtime ;
	
	gps->getDateString (&day, &mon, &yr, &gtime) ;
	sprintf (prefx, "%04d%02d%02d_%06ld",yr, mon, day, gtime) ; 
 
}
