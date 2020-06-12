
TARGET:=   streamspy
CC=        g++
DEF=      -D__DEBUG__ -DHAVE_CXX_STDHEADERS
#INCLUDE=  -I./libbase -I../ffmpeg-3.3/include -I./jsoncpp/include
INCLUDE=  -I/usr/include -I./jsoncpp
LIBS=     -L/usr/lib -lavformat -lavcodec -lavutil -l avfilter
#LIBS=      -L../ffmpeg-3.3/lib -lavformat
CPPFLAGS= -g -Wall -ldl -lpthread ${DEF} ${INCLUDE} 
OBJS= 	   Analyzer.o main.o jsoncpp/json_reader.o jsoncpp/json_value.o jsoncpp/json_writer.o

.cpp .o:

all: ${TARGET}

${TARGET}: ${OBJS} 
	${CC} ${CPPFLAGS} ${OBJS} -o ${TARGET} ${LIBS}

install:
	@cp ${TARGET}	../bin	-rf

clean:
	@rm ${OBJS} -rf
	@rm ${TARGET}  -rf
