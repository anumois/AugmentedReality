
INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

CC=gcc
LDFLAG=-L/usr/lib/x86_64-linux-gnu -L/usr/lib64 -L$(LIB_DIR)/linux-x86_64 -L$(LIB_DIR)
LIBS= -lARgsub_lite -lARvideo -lAR -lARICP -lAR -lglut -lGLU -lGL -lX11 -lm -lpthread -ljpeg
CFLAG= -O3 -fPIC -march=core2 -DHAVE_NFT=1 -I/usr/include/x86_64-linux-gnu -I$(INC_DIR)

OBJS =
HEADERS = 

all: $(BIN_DIR)/ARappServer

$(BIN_DIR)/ARappServer: ARappServer.o ASMessage.o ASSocket.o ASSharedData.o ASsetup.o $(OBJS)
	cc -o $(BIN_DIR)/ARappServer ARappServer.o ASMessage.o ASSocket.o ASSharedData.o ASsetup.o $(OBJS) $(LDFLAG) $(LIBS)

ARappServer.o: ARappServer.c $(HEADERS)
	cc -c $(CFLAG) ARappServer.c

ASMessage.o: ASMessage.c $(HEADERS)
	cc -c $(CFLAG) ASMessage.c

ASSharedData.o: ASSharedData.c $(HEADERS)
	cc -c $(CFLAG) ASSharedData.c

ASSocket.o: ASSocket.c $(HEADERS)
	cc -c $(CFLAG) ASSocket.c

ASsetup.o: ASsetup.c $(HEADERS)
	cc -c $(CFLAG) ASsetup.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/ARappServer

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/ARappServer
	rm -f Makefile

distclean:
	rm -f *.o
	rm -f Makefile
