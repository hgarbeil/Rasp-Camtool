#include "avaspec.h"
#include <stdio.h>
#include <sys/types.h>
#include "unistd.h"
//#include <thread>
#include <iostream>
#include "Avantes.h"



using namespace std;

double specData [3648] ;
bool dataReady ;
namespace Avantes {

AvsIdentityType a_pList[2] ;
MeasConfigType l_PrepareMeasData [2] ;
AvsHandle spec[2] ;

short nscans_dark, nscans_collect ;
int nCollected ;
int nspecs, avs_status ;
unsigned short npix ;
double *dark ;
/*
int avantes_init () ;
void avantes_closeup () ;
void avantes_initMeasStruct (int specNum);
void avantes_collectDark () ;
void avantes_darkCB (AvsHandle *av, int *result) ;
int main(int argc, char *argv[]) ;
*/

int avantes_init () {
    int status ;
    unsigned int bset ;
    nscans_collect = 10 ;
    nscans_dark = 25 ;
    npix = 3648 ;
    nCollected = 0 ;
    dataReady = false ;
    avs_status = AVS_Init (0) ;

    cout << "Spectrometer init status is : " << avs_status << endl ;

    nspecs = AVS_GetNrOfDevices() ;

    nspecs = AVS_GetList (sizeof(a_pList), &bset, a_pList) ;
    cout << "Num returned from get list "<< nspecs << endl ;
    if (nspecs < 1) {
	cout << "Couldn't get a spectrometer " << endl ;
    	return (0) ;
    } 
    cout << "Spec0 ser num is : "<< a_pList[0].SerialNumber << endl ;
    spec[0] = AVS_Activate (&a_pList[0]) ;
    cout << "Activate returns handle of : " << spec[0] << endl ;
    avs_status = AVS_GetNumPixels (spec[0], &npix) ;
    cout << "Number of npix is : " << npix << endl ;

    Avantes::avantes_initMeasStruct(0);
    Avantes::avantes_initMeasStruct(1);

    dark = new double [npix] ;
    
    status = AVS_PrepareMeasure (spec[0], &l_PrepareMeasData[0]) ;
    cout << "AVS_PrepareMeasure status is : " << status << endl ;
    usleep (1000000) ;

    return (nspecs) ;   
}

void avantes_resetDark () {
	for (int i=0; i<npix; i++) dark[i]=0. ;
}

void avantes_closeup () {

    AVS_Deactivate(spec[0]) ;
    AVS_Done() ;
    delete [] dark ;
}


void avantes_initMeasStruct (int specNum){
        l_PrepareMeasData[specNum].m_StartPixel = 0 ;
        l_PrepareMeasData[specNum].m_StopPixel = npix-1 ;
        l_PrepareMeasData[specNum].m_IntegrationTime = 100;
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

void avantes_collectDark () {
        //AVS_StopMeasure (spec[0]) ;
        int status ;
	int count = 0 ;
	

    	avantes_resetDark() ;
	nCollected = 0 ;
	status = 1 ;
	while (status!=0) {
            status = AVS_MeasureCallback (spec[0], Avantes::avantes_darkCB, nscans_dark) ;
		usleep (100000) ;
	}
        cout << "measure status is "  << status << endl ;
	while (nCollected < nscans_dark && count < 1000) {
                count++ ;
        	usleep (10000) ;
	}
        AVS_StopMeasure (spec[0]) ;
}


void avantes_darkCB (AvsHandle *av, int *result) {
    unsigned int timLabel ;

    AVS_GetScopeData(*av, &timLabel, specData) ;
    dataReady = true ;
    if (nCollected!=0)
    {
    	for (int i=0; i<npix; i++) {
		dark[i] += specData[i]/(nscans_dark-1) ;
    	}
    }
    nCollected++ ;
    cout << "Spec num 300 "<< dark[300] << endl ;
}

}

//int main(int argc, char*argv[]) {
//	int status ;
//	status = avantes_init() ;
//	if (status < 1) {
//		return (-1) ;
//	}
//	avantes_collectDark() ;
//	avantes_closeup() ;

//}
