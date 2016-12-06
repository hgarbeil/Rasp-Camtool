#include "Avaspec.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std ;

Avaspec::Avaspec() {
	int i ;
	unsigned int ByteSet ;
	goThread = false ;

	waves = 0l ;
	specData = 0L ;





}

void Avaspec::start () {
	int status ;
	goThread = true ;
	status = AVS_Measure (spec[0], NULL, -1) ;
	m_thread = std::thread (&Avaspec::startCollecting, this) ;

}	
void Avaspec::stop () {
	goThread = false ;
}

int Avaspec::init() {
	int i, status ;
	unsigned short num ;
	//printf ("Number of mini-avs specs found %n\r\n", nspecs) ;
	int n = AVS_Init(0) ;
	nspecs = AVS_GetNrOfDevices() ;
	nspecs = AVS_GetList (sizeof (a_pList), &ByteSet, a_pList) ;
	if (nspecs < 1) {
		cout << "No mini-avs specs found" << endl ;
		return (-1) ;
	}
	
	printf ("Number of mini-avs specs found %d\r\n", nspecs) ;
	spec = new AvsHandle [nspecs]  ;
	spec[0] = AVS_Activate (&a_pList[0]) ;
	spec[1] = AVS_Activate (&a_pList[1]) ;
	
	if (nspecs > 0) {
		npix = AVS_GetNumPixels (spec[0], &num) ;
		printf ("Number of pixels is %d\r\n", num) ;
	}

	l_PrepareMeasData.m_StartPixel = 0 ;
	l_PrepareMeasData.m_StopPixel = num-1 ;
	l_PrepareMeasData.m_IntegrationTime = 100 ;
	l_PrepareMeasData.m_IntegrationDelay = 0 ;
	l_PrepareMeasData.m_NrAverages = 1 ;
    	l_PrepareMeasData.m_CorDynDark.m_Enable = 0;
    	l_PrepareMeasData.m_CorDynDark.m_ForgetPercentage = 0;
    	l_PrepareMeasData.m_Smoothing.m_SmoothPix = 0;
    	l_PrepareMeasData.m_Smoothing.m_SmoothModel = 0;
    	l_PrepareMeasData.m_SaturationDetection = 0;
    	l_PrepareMeasData.m_Trigger.m_Mode = 0;
    	l_PrepareMeasData.m_Trigger.m_Source = 0;
    	l_PrepareMeasData.m_Trigger.m_SourceType = 0;
    	l_PrepareMeasData.m_Control.m_StrobeControl = 0;
    	l_PrepareMeasData.m_Control.m_LaserDelay = 0;
    	l_PrepareMeasData.m_Control.m_LaserWidth = 0;
    	l_PrepareMeasData.m_Control.m_LaserWaveLength = 0;
    	l_PrepareMeasData.m_Control.m_StoreToRam = 0;



	status = AVS_PrepareMeasure (spec[0], &l_PrepareMeasData) ;
	
	waves = new double [nspecs * num] ;
	for (i=0; i<nspecs; i++) {
		AVS_GetLambda (spec[i], &waves[i * num]) ;
	}
		
	specData = new double [num * nspecs] ;
	ofstream ofil;
	ofil.open ("waves.txt") ;
	for (i=0; i<num; i++) {
		ofil << i << "  " << waves[i]<< "  "<< waves[num+i] << endl ;
	}
	ofil.close() ;


	return (1) ;

}	

void Avaspec::setPixels (int specnum, int start, int stop) {
	l_PrepareMeasData.m_StartPixel = start ;
	l_PrepareMeasData.m_StopPixel = stop ;

	AVS_PrepareMeasure (spec[specnum], &l_PrepareMeasData) ; 


}

void Avaspec::startCollecting () {

	int status ;
	unsigned int timLabel ;

	while (goThread) {
		status = AVS_PollScan (spec[0]) ;
		//cout << "status is " << status << endl ;
		if (status) {
			AVS_GetScopeData (spec[0], &timLabel, specData) ;
			cout << timLabel << " " << specData[300] << endl ;
		} 
		else {
			usleep (10000) ;
		}
	}

}

Avaspec::~Avaspec() {
	AVS_StopMeasure (spec[0]) ;
	AVS_Done() ;
	delete [] spec ;
	delete [] specData ;
	delete [] waves ;

}

int main() {


	Avaspec *aspec = new Avaspec() ;
	aspec->init() ;
	aspec->start() ;
	while (1) {
		usleep (1000000) ;

	}

}    
