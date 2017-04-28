#include "GPIO.h"
#include <iostream>
using namespace std ;

GPIO::GPIO() {
	init() ;

	

}


GPIO::~GPIO() {
	gpioSetMode (18, 0) ;
	gpioTerminate() ;
}

void GPIO::init () {

	int status = gpioInitialise() ;  
	cout << "GPIO Init status is : " << status << endl ;

	gpioSetMode (18, PI_OUTPUT) ; 

}

void GPIO::toggle (int state) {

	gpioWrite (18, state) ;
	//gpioWrite (18, 0) ;

}
