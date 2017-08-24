# AugmentedReality

Test Environment: Ubuntu 16.04.2 LTS

## Used libraries

### ARToolKit required

ARToolKitv5 library already included in /include directory.

ARToolKit SDK can be downloaded from https://archive.artoolkit.org/download-artoolkit-sdk

### openGL required

sudo apt-get install freeglut3-dev

### libglc required

libglc required to print text via openGL

sudo apt-get install build-essential cmake libx11-dev libxxf86vm-dev libgl1-mesa-dev libasound2-dev libpng12-dev

sudo apt-get install gcc-multilib

sudo ln -s /usr/lib32/libGL.so.1 /usr/lib32/libGL.so

sudo ln -s /usr/lib32/libasound.so.2 /usr/lib32/libasound.so

sudo ln -s /usr/lib32/libXxf86vm.so.1 /usr/lib32/libXxf86vm.so

sudo ln -s /usr/lib32/libX11.so.6 /usr/lib32/libX11.so

sudo ln -s /usr/lib32/libpng12.so.0 /usr/lib32/libpng.so

sudo ln -s /usr/lib64/libglc-capture.so.0 /usr/lib/libglc-capture.so.0

sudo ln -s /usr/lib64/libglc-capture.so.0 /usr/lib/libglc-capture.so.

sudo ln -s /usr/lib64/libglc-core.so.0 /usr/lib/libglc-core.so.0

sudo ln -s /usr/lib64/libglc-core.so.0 /usr/lib/libglc-core.so

sudo ln -s /usr/lib64/libglc-export.so.0 /usr/lib/libglc-export.so.0

sudo ln -s /usr/lib64/libglc-export.so.0 /usr/lib/libglc-export.so

sudo ln -s /usr/lib64/libglc-hook.so.0 /usr/lib/libglc-hook.so.0

sudo ln -s /usr/lib64/libglc-hook.so.0 /usr/lib/libglc-hook.so

sudo ln -s /usr/lib64/libglc-play.so.0 /usr/lib/libglc-play.so.0

sudo ln -s /usr/lib64/libglc-play.so.0 /usr/lib/libglc-play.so

sudo ln -s /usr/lib64/libpacketstream.so.0 /usr/lib/libpacketstream.so.0

sudo ln -s /usr/lib64/libpacketstream.so.0 /usr/lib/libpacketstream.so

## Sources

Our work done under /src directory

### ARappClient

#### Workflow

1.   Client sends socket request to the server
2.   Server sends marker ID information back to the client
2-1. The client then creates a thread that only receives packets coming from the server
3.   When the game starts the client searches for any marker information
     that is recoreded in the system
3-1. When a generator marker is detected, has the ball and keyboard 'g' is pressed,
     the client sends "GET" signal to the server. When server allows the client to
     get the ball from the marker, it informs every clients with "BROADCAST"
3-2. The same goes for "STEAL"ing the ball from other player's marker and
     "ShOOT"ing the ball into the goal post marker.
3-3. The game should end after some interval of time.

#### Files

##### ARappClient.c

This file holds the main function and the main loop function that is executed periodically.
It initialize libraries and data structure in order to start the client application
In the main loop, from the video received, it detects each marker and performs dedicated actions

##### ACSetup.c

This file holds the initialization functions used for the client application.
It initialzes network socket, camera, and marker patterns.

##### ACFigures.c

This file holds openGL functions that is used for drawing objects on screen

##### ACMessage.c

This file holds functions used to communicate with the server.

##### ACSharedData.c

This file holds information that should be shared with the main process and the
data receiving thread

##### ACThread.c

This file holds functions that the thread (used only for receiving data) needs.
The thread process incoming data from the server and applies to all-shared data

#### ARappServer

This is the server application that controls the overall gameflow

##### ARappServer.c

This file holds the main function of the server application.
It initializes the network socket and creates childs for every incoming clients.

##### ASMessage.c

This file holds functions that is used to communicate with the client

##### ASSharedData.c

This file holds information that should be shared with the each of the forked child processes.

##### ASsetup.c

This file holds initialization functions

##### ASSocket.c

This file holds functions that each forked child process runs. The child process communicates with the client using the functions from ASMessage.c


### Limitations

1. Marker recognition has distance limitations. When using a regular webcam,
   the recognition can be done as far as 5~8 meters, not enough when using it with
   drones
2. There is an extreme lag issues when running the client. This probably has to do with
   openGL functions used and multiple marker recognition. It has to be fixed.

### Further Approach

Using GPS module to recognize other drones and symbols would be a great way to solve limitation 1. 


Written by 2017 Summer undergraduate individual studies team