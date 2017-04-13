#include "phidgetsMot.h"
#include "unistd.h"
#include "math.h"

phidgetsMot::phidgetsMot () {

    dark = 40 ;
    low = 80 ;
    high = 120 ;
    ref = 150 ;

    nmotors = 1 ;
    caloff = 0 ;
    scanoff = 0 ;
    scanscale = 1. ;
    cellSer = 0 ;
    scanSer = 0 ;

	servo = 0 ;
    servo1 = 0 ;
    servoCell = 0 ;
    servoScan = 0 ;

    startAng = 20 ;
    endAng = 160 ;
    incAng = 10 ;
    curAng = 90 ;
    lastPos = 90 ;
    firstTimeMirrorFlag = true ;
	init() ;

}


void phidgetsMot::init(){
    int result ;
    const char *err ;
    int ser0, ser1 ;

    ser0 = -1 ;
    ser1 = -1 ;
    CPhidgetServo_create (&servo) ;
    //CPhidgetAdvancedServo_create (&servo1) ;
    CPhidget_open ((CPhidgetHandle) servo, -1) ;

    printf("Waiting for Servo controller to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)servo, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
            return ;

    }
    CPhidget_getSerialNumber ((CPhidgetHandle) servo, &ser0) ;
    servoCell = servo ;
    //CPhidgetAdvancedServo_create (&servo1) ;
    if (nmotors ==1) return ;
    CPhidget_open ((CPhidgetHandle) servo1, -1) ;

    printf("Waiting for Servo controller to be attached....");
    if (nmotors==2) {
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)servo1, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
            return ;

    }
    CPhidget_getSerialNumber ((CPhidgetHandle) servo1, &ser1) ;
    }

    if (ser0 == cellSer) {
        servoCell = servo ;
        servoScan = servo1 ;
    }
    else {
        servoScan = servo ;
        servoCell = servo1 ;
    }

    //qDebug () << "CELL : " << cellSer << "\n";


}


phidgetsMot::~phidgetsMot () {
	CPhidgetServo_setEngaged (servo, 0, 0) ;
	CPhidget_close ((CPhidgetHandle) servo) ;
	CPhidget_delete ((CPhidgetHandle) servo) ;
}

void phidgetsMot::setSerialNumbers(int c, int s){
    cellSer = c ;
    scanSer = s ;
}

void phidgetsMot::setPosition (CPhidgetServoHandle curservo, int pos){
    int diffang, ival, sleepval ;
    float newPos, incid ;
    CPhidgetServo_setEngaged (curservo, 0, 1) ;

    if (curservo == servoCell) {
        CPhidgetServo_setPosition (curservo, 0, pos) ;
        usleep (800000) ;
        //sleep (1) ;
    }
    else {
        incid = pos - 90. ;
        // add in 30 degree offset
        newPos = 120 + scanoff + (incid * scanscale) ;
        diffang = fabs (newPos - lastPos) ;
        sleepval = 100000 * (1+diffang/20.) ;
        if (firstTimeMirrorFlag) {
            sleepval = 800000 ;
            firstTimeMirrorFlag = false ;
        }
        CPhidgetServo_setPosition (curservo, 0, newPos) ;
        //printf ("Pos: %d   ScaledPos : %d  Sleep %d\r\n", pos, (int)newPos, sleepval/1000) ;
        //fflush(stdout);
        if (sleepval >= 1000000) sleepval = 900000 ;
        usleep (sleepval) ;
        lastPos = newPos ;
    }

    //usleep (200000) ;
    CPhidgetServo_setEngaged (curservo, 0, 0) ;

}
	
// set motor positions for the reference
void phidgetsMot::setCalPositions (int d, int l, int h, int r){
    dark = d + caloff;
    low = l + caloff ;
    ref = r + caloff ;
    high = h +caloff;
}

void phidgetsMot::setDark  (){

    emit (0) ;
    setPosition (servo, dark) ;


}
void phidgetsMot::setLow  (){

    emit (1) ;
    setPosition (servoCell, low) ;

}
void phidgetsMot::setHigh  (){

    emit (2) ;
    setPosition (servoCell, high) ;

}
void phidgetsMot::setRef  (){

    emit (3) ;
    setPosition (servoCell, ref) ;


}

void phidgetsMot::setMotors (int n, float coff, float soff, float sscale) {
    nmotors = n ;
    scanoff = soff ;
    caloff = coff ;
    scanscale = sscale ;

}

void phidgetsMot::setScanScaleOff (float soff, float sscale) {

    scanoff = soff ;
    scanscale = sscale ;

}


void phidgetsMot::setMirrorAngles (int st, int en, int inc){
    /*
    startAng = int(float(st + scanoff) * scanscale) ;
    endAng = int(float(en + scanoff) * scanscale);
    incAng = int(float(inc)* this->scanscale) ;
    */
    startAng = st ;
    endAng = en ;
    incAng = inc ;

}


void phidgetsMot::scanIncrement (){

    curAng += incAng ;
    if (curAng > endAng) {
        curAng = startAng ;
    }
    this->setPosition (servoScan, curAng) ;
}


void phidgetsMot::scanHome (){

    curAng = 90 ;
    this->setPosition (servoScan, curAng) ;
}

void phidgetsMot::scanSet (int ang){

    curAng = ang ;
    this->setPosition (servoScan, ang) ;
}

void phidgetsMot::emit (int state){
	printf ("Cell state is %d\r\n", state) ;
}

