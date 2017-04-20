CC= g++ 

FLAGS = -g
CCFLAGS = ${FLAGS} -std=c++11


OBJS = main.o Avaspec.o GPIO.o phidgetsMot.o GPS.o IMUThread.o 
LIBS = -L/usr/local/lib -lavs -lpthread -lpigpio -lphidget21 -lgps -lRTIMULib
INCLUDES = -I/usr/local/include/libavs -I.. 
EXECUTABLE = GasSpec

.cpp.o :
	${CC} ${CCFLAGS} -c ${INCLUDES} $*.cpp

${EXECUTABLE} : ${OBJS}
	${CC} ${CCFLAGS} ${OBJS} -o ${EXECUTABLE} ${LIBS}

clean :
	/bin/rm -f ${OBJS} ${EXECUTABLE}
