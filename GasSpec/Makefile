CC= g++ 

FLAGS = -g
CCFLAGS = ${FLAGS} -std=c++11


OBJS = main.o Avaspec.o GPIO.o
LIBS = -L/usr/local/lib -lavs -lpthread -lpigpio
INCLUDES = -I/usr/local/include/libavs 
EXECUTABLE = GasSpec

.cpp.o :
	${CC} ${CCFLAGS} -c ${INCLUDES} $*.cpp

${EXECUTABLE} : ${OBJS}
	${CC} ${CCFLAGS} ${OBJS} -o ${EXECUTABLE} ${LIBS}

clean :
	/bin/rm -f ${OBJS} ${EXECUTABLE}
