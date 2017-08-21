# AugmentedReality

## libglc required
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

LIBS= -lARgsub_lite -lARvideo -lAR -lARICP -lAR -lglut -lGLU -lGL -lGLC -lX11 -lm -lpthread -ljpeg -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -pthread -lgthread-2.0 -pthread -lglib-2.0 -lxml2
CFLAG= -O3 -fPIC -march=core2 -DHAVE_NFT=1 -I/usr/include/x86_64-linux-gnu -pthread -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libxml2 -I$(INC_DIR)

