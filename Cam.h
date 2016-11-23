#include <raspicam/raspicam.h>
#include <fstream>
#include <iostream>

using namespace raspicam ;

class CamC {
public :
	CamC () ;
	~CamC () ;
	bool camopen () ;
	void getImage (unsigned char *d) ;
	void writeJPeg (const char *fname, unsigned char *) ;
	void camparams() ;
	RaspiCam *cam ;
	int h, w ;

} ;
	
