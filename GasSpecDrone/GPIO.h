#include <pigpio.h>
#ifndef pgio
#define pgio

class GPIO {

public :

	GPIO();
	~GPIO();
	void init() ;
	void toggle (int) ;
} ; 
	
#endif
