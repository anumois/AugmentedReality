
INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

CC=gcc
LDFLAG=-L/usr/lib/x86_64-linux-gnu -L/usr/lib64 -L$(LIB_DIR)/linux-x86_64 -L$(LIB_DIR)
LIBS= -lARgsub_lite -lARvideo -lAR -lARICP -lAR -lglut -lGLU -lGL -lGLC -lX11 -lm -lpthread -ljpeg -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 -lxml2
CFLAG= -O3 -fPIC -march=core2 -DHAVE_NFT=1 -I/usr/include/x86_64-linux-gnu -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libxml2 -I$(INC_DIR)

OBJS =
HEADERS =

all: $(BIN_DIR)/ARappClient

$(BIN_DIR)/ARappClient: ARappClient.o ACMessage.o ACSetup.o ACThread.o ACSharedData.o ACFigures.o $(OBJS)
	cc -o $(BIN_DIR)/ARappClient ARappClient.o ACMessage.o ACSetup.o ACThread.o ACSharedData.o ACFigures.o $(OBJS) $(LDFLAG) $(LIBS)

ARappClient.o: ARappClient.c $(HEADERS)
	cc -c $(CFLAG) ARappClient.c

ACMessage.o: ACMessage.c $(HEADERS)
	cc -c $(CFLAG) ACMessage.c

ACSharedData.o: ACSharedData.c $(HEADERS)
	cc -c $(CFLAG) ACSharedData.c

ACSetup.o: ACSetup.c $(HEADERS)
	cc -c $(CFLAG) ACSetup.c

ACThread.o: ACThread.c $(HEADERS)
	cc -c $(CFLAG) ACThread.c

ACFigures.o: ACFigures.c $(HEADERS)
	cc -c $(CFLAG) ACFigures.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/ARappClient
