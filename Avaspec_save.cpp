#include "Avaspec.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <QDebug>

using namespace std ;

Avaspec::Avaspec() {
	int i ;
	goThread = false ;
	needsUpdate = new bool [5] ;
	needsUpdate[0] = true ;
	needsUpdate[1] = true ;
	needsUpdate[2] = true ;


	waves = 0l ;
	specData = 0L ;

	npix = 3648 ;
    nscansCollect = 500000 ;
    AVS_UpdateUSBDevices() ;


}

int Avaspec::init() {
    int i, status ;
    unsigned int ByteSet ;
    unsigned short num ;
    AvsHandle s0 ;

    //printf ("Number of mini-avs specs found %n\r\n", nspecs) ;
    int n = AVS_Init(0) ;
    if (n<0) {
        qDebug () <<"AVS Init failed" ;
        return (-1);
    }
    nspecs = AVS_GetNrOfDevices() ;
    nspecs = AVS_GetList (sizeof (AvsIdentityType)*nspecs, &ByteSet, &a_pList[0]) ;
    if (nspecs < 1) {
        qDebug() << "No mini-avs specs found" << endl ;
        return (-1) ;

    }

    qDebug() << "Number of mini-avs specs found :" << nspecs ;
    l_PrepareMeasData = new MeasConfigType [nspecs] ;
    // initialize the spec structure
    for (i=0; i<nspecs; i++){
        //spec[i] = AVS_Activate (&a_pList[i]) ;
        spec[i] = AVS_Activate (&a_pList[i]) ;
        AVS_GetNumPixels (spec[i], &num) ;
        initMeasStruct (i) ;
        status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
        needsUpdate[i] = false ;
    }

//    if (nspecs > 0) {

//        printf ("Number of pixels is %d\r\n", num) ;
//    }

    npix = num ;
    waves = new float [nspecs*npix];
    double *dwaves = new double [nspecs * npix] ;
    for (i=0; i<nspecs; i++) {
        AVS_GetLambda (spec[i], &dwaves[i * npix]) ;
        for (int is=0;is<npix; is++)
            waves[i*npix+ is] = (float) dwaves [i*npix+is] ;
    }
    delete [] dwaves ;

    specData = new double [num * nspecs] ;
    ofstream ofil;
    ofil.open ("waves.txt") ;
    for (i=0; i<num; i++) {
        ofil << i << "  " << waves[i]<< "  "<< waves[num+i] << endl ;
    }
    ofil.close() ;


    return (1) ;

}


void Avaspec::start () {
	int i, status ;
	goThread = true ;

	for (i=0; i<nspecs; i++) 
	{
		if (needsUpdate[i]) {
			status = AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
			needsUpdate[i] = false ;
		}
        //status = AVS_StopMeasure(spec[i]) ;
		status = AVS_Measure (spec[i], NULL, -1) ;
	}
	m_thread = std::thread (&Avaspec::startCollecting, this) ;

}	
void Avaspec::stop () {
	goThread = false ;
}


void Avaspec::setScanData (int nscans, float *d) {
    this->outdat = d ;
    this->nscansCollect = nscans ;
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

void Avaspec::startCollecting () {

    int i, is, status, iter = 0, count=0 ;
	unsigned int timLabel ;

    while (goThread & iter<this->nscansCollect) {
		status = AVS_PollScan (spec[0]) ;
        cout << "status is " << status << endl ;
		if (status) {
			AVS_GetScopeData (spec[0], &timLabel, specData) ;
            for (is=0; is<npix; is++) this->outdat[npix * count + is] = (float)specData[is] ;
			cout << timLabel << " " << specData[300] << endl ;
            iter++ ;
            emit (gotData());
		} 
		else {
			usleep (10000) ;
		}
		for (i=0; i<nspecs; i++)
		if (needsUpdate[i]) {
			AVS_StopMeasure(spec[i]) ;
			AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
			AVS_Measure (spec[i], NULL, -1) ;
			needsUpdate[i] = false ;
		}

	}
    goThread = false ;
    pthread_exit (NULL) ;
    //for (i=0; i<nspecs; i++)
    //    AVS_StopMeasure (spec[i]) ;

}

void Avaspec::getSingleScan (float *data) {
    int i, is, count=0, iter=0 ;
    unsigned int timLabel ;


    if (nspecs <1) return ;
    for (i=0; i<nspecs; i++) {
        if (needsUpdate[i]) AVS_PrepareMeasure (spec[i], &l_PrepareMeasData[i]) ;
        AVS_MeasureCallback (spec[i], NULL, -1) ;
        needsUpdate[i] = false ;
     }
     while (count<nspecs & iter <500) {
        if (!AVS_PollScan(spec[count])){
             iter++ ;
             usleep (1000) ;
             continue ;
         }
         AVS_GetScopeData (spec[count], &timLabel, specData) ;
         for (is=0; is<npix; is++) data[npix * count + is] = (float)specData[is] ;
         count++ ;
     }
     for (i=0; i<nspecs; i++)
        AVS_StopMeasure (spec[i]) ;

}


void Avaspec::dataCallback (AvsHandle *av, int *result) {
    qDebug() << *result << "  Wahoo " ;
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
