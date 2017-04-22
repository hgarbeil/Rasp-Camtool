CC = g++ -g -std=c++11
LIBS = -L ../libgps/build -lgps -lRTIMULib -lraspicam -ljpeg -lpthread -lm
INCS = -I ../libgps/src
OBJS = IMUThread.o GPS.o Cam.o main.o clientTCP.o
OBJS1 = Cam.o

.cpp.o : 
	${CC} -c ${INCS} $*.cpp
camtest : ${OBJS1}
	${CC} ${OBJS1} -o camtest ${LIBS}

camtool : ${OBJS}
	${CC} ${OBJS} -o camtool ${LIBS}
	




