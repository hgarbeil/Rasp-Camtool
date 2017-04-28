CC= g++ 

FLAGS = -g
CCFLAGS = ${FLAGS} -std=c++11


OBJS = main.o Avaspec.o GPIO.o phidgetsMot.o GPS.o IMUThread.o DroneData.o 
LIBS = -L../sys/lib -L/usr/local/lib -L/home/pi/dji2/Onboard-SDK/build/libs -lavs -lpthread -lpigpio -lphidget21 -lgps -lRTIMULib -ldjiosdk-core -ldjiosdk-platform -ldjiosdk-wrapper

INCLUDES = -I../sys/include/libavs -I../sys/include -I/usr/local/include/libavs -I.. \
	-I/home/pi/dji2/Onboard-SDK/osdk-core/inc -I/home/pi/dji2/Onboard-SDK/osdk-wrapper/inc \
	 -I/home/pi/dji2/Onboard-SDK/platform/linux/inc

EXECUTABLE = GasSpec

.cpp.o :
	${CC} ${CCFLAGS} -c ${INCLUDES} $*.cpp

${EXECUTABLE} : ${OBJS}
	${CC} ${CCFLAGS} ${OBJS} -o ${EXECUTABLE} ${LIBS}

clean :
	/bin/rm -f ${OBJS} ${EXECUTABLE}
