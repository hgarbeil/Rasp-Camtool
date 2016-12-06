#include <stdio.h>
#include <sys/types.h>
#include <thread>
#include "avaspec.h"

using namespace std ;
class Avaspec {
public :


	Avaspec() ;
	~Avaspec() ;
	
	int init() ;
	//void getSpectrometer (int specnum) ;
	AvsIdentityType a_pList[30] ;
	AvsHandle *spec ;
	double *waves ;
	double *specData ;
	int nspecs ;
	int npix ;
	void startCollecting() ;
	void setPixels (int specnum, int start, int stop) ;
	void start() ;
	void stop () ;
	thread m_thread ;
	bool goThread ;
	MeasConfigType l_PrepareMeasData;


} ;

