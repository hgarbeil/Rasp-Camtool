#ifndef AVANTES_H
#define AVANTES_H

#include "avaspec.h"


namespace Avantes {

int avantes_init () ;
void avantes_closeup () ;
void avantes_initMeasStruct (int specNum);
void avantes_collectDark () ;
void avantes_darkCB (AvsHandle *av, int *result) ;
}
#endif // AVANTES_H
