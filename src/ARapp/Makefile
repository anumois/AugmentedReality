
INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

CC=gcc
LDFLAG=-L/usr/lib/x86_64-linux-gnu -L/usr/lib64 -L$(LIB_DIR)/linux-x86_64 -L$(LIB_DIR)
LIBS= -lARgsub_lite -lARvideo -lAR -lARICP -lAR -lglut -lGLU -lGL -lX11 -lm -lpthread -ljpeg
CFLAG= -O3 -fPIC -march=core2 -DHAVE_NFT=1 -I/usr/include/x86_64-linux-gnu -I$(INC_DIR)

OBJS =
HEADERS = 

all: $(BIN_DIR)/ARapp

$(BIN_DIR)/ARapp: ARapp.o setup.o figures.o client.o sharedData.o $(OBJS)
	cc -o $(BIN_DIR)/ARapp setup.o ARapp.o figures.o client.o sharedData.o $(OBJS) $(LDFLAG) $(LIBS)

ARapp.o: ARapp.c $(HEADERS)
	cc -c $(CFLAG) ARapp.c

setup.o: setup.c $(HEADERS)
	cc -c $(CFLAG) setup.c

figures.o: figures.c $(HEADERS)
	cc -c $(CFLAG) figures.c

client.o: client.c $(HEADERS)
	cc -c $(CFLAG) client.c

sharedData.o: sharedData.c $(HEADERS)
	cc -c $(CFLAG) sharedData.c


clean:
	rm -f *.o
	rm -f $(BIN_DIR)/ARapp

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/ARapp
	rm -f Makefile

distclean:
	rm -f *.o
	rm -f Makefile
