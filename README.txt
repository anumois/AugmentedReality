Read me for ARapp using ARToolKit5.
===================================



Contents.
---------

About this application.
Set up.
Running the application.
Known errors.
Next steps.
Explanations how ARToolKit works.



About this application.
-----------------------

Final goal of this application is to make drone quidditch using Augmented Reality.

ARToolKit5 is used to track markers and calculate their orientation. Used OpenGL to draw virtual objects over camera view. For the input video stream use GStreamer as drone uses it, too.

This application is made by:
     Park Jinwoo
     Kim Jae Hong
     Do Gi Hong
     2017-08-25



Set up.
-------

-For SimpleLite.
Install ARToolKit5 SDk.
After unpack, open terminal window, and run the script artoolkit5-setenv: (Below example assumes ARToolKit5 is in ~/SDKs/)
      cd ~/SDKs/ARToolKit5/
      ./share/artoolkit5-setenv
To use a V4L2 camera, do
   export ARTOOLKIT5_VCONF="-device=LinuxV4L2"
To use a camera driven via GSTreamer, do
   export ARTOOLKIT5_VCONF="-device=GStreamer"
cd to the bin directory and do
   ./simpleLite

-For Application.
Install OpenGl "libgl1-mesa-dev"
Install OpenCV "libopencv-dev"
Install V4l2 "libv4l2-dev"
Install Gstreamer0.10 "libgstreamer0.10-dev"
Install gstreamer plugins-good "gstreamer0.10-plugins-good"
Building proceeds with the usual steps "./Configure; make". During the configure process, select GStreamer capture module to build.



Running the application.
------------------------

After compiling, the executables for application can be found in the bin directory. ARappServer is first to be ran. ARappClient should be ran after ARappServer with server's IP address. Since ARappClient is assuming to receive video from video configuration should be set accordingly. After setting vconf and appropriately send video to right port, the application is off to go.

For example, one should edit vconf in main function of ARappClient.c
   vconf[] = "udpsrc port=5000 ! application/x-rtp, encoding-name=JPEG, payload=96 ! rtpjpegdepay ! jpegdec ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink";
In order to send video to receving side, one should open terminal window and command like below.
   gst-launch-0.10 -v v4l2src device=/dev/video0 ! ffmpegcolorspace ! video/x-raw-yuv, width=640, height=480, framerate=30/1 ! ffmpegcolorspace ! jpegenc ! rtpjpegpay ! udpsink host=localhost port=5000

Since this is only example, pipelines should be changed according to specific conditions. If you are directly using video from the camera connected to the client, one possible example would be shown below.
   vconf[] = "v4l2src device=/dev/video0 use-fixed-fps=false ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24,framerate=5/1 ! identity name=artoolkit sync=true ! fakesink";

It is also possible to change patterns, aka traditional template marker. In the main() right below vconf, patt_name and it's path are setted with our default data and information. The patterns can be easily changed if these path are changed.



Known errors.
-------------

Port ARToolKit5 to GStreamer1.0 from GStreamer0.10
https://github.com/artoolkit/artoolkit5/pull/182/commits
"GStreamer-CRITICAL **: gst_mini_object_unref: assertion 'mini_object->refcount>0' failed"
Freeing memory of the possible square marker has problem. If the video capture does not contain any possible form of square marker, the error message does not come out. The application is killed after 5-10min later due to the memory leak.

Detection distance using A4 size template traditional marker is about 5meters.
MANUAL THRESHOLD: 95
MANUAL ADAPTIVE
MANUAL OTSU
MANUAL THRESHOLD: 120(default)
is the order of the well-detected options when experimented indoor.
ARToolKit5 has very low rate of detecting the marker when marker has light reflection. Especially natural sunlight is critical if you use common printer with common A4 paper.



Next steps.
-----------

Port ARToolKit to GStreamer1.0 from GStreamer0.10.
Change ARToolKit's marker recognition module to enhance marker detection.
Use GPS data and apply non-marker based Augmented Reality. It will probably have no relationship with ARToolKit at all.
Make better 3D objects without taking much computational power.



Explanations how ARToolKit works.
---------------------------------

-Coordinates of camera view

Origin: Center of the video input
Distance unit: Pre-determined pattern size
X coordinate: gPatt_trans[0][3] (horizontal)
Y coordinate: gPatt_trans[1][3] (vertical)
Z coordinate: gPatt_trans[2][3] (depth)


-Detecting Pattern using traditional template marker (SimpleLite)

In each frame, every possible black markers are stored in gARHandle -> markerInfo[j].
The number of detected black markers is stored in gARHandle -> marker_num.
In order to determine marker found in the frame as one of pre-uploaded pattern,
ARToolKit compare markerinfo[j].id and gPatt_id.


-Unit size used in the application.

Application itself does not have ability to measure actual size of markers nor distances. It processes relative distances and size compared to pre-defined gPatt_width size. All the OpenGL units also uses same data. To change the absolute units, it can be achieved by changing gPatt_width's static value.


-Setting video configuration properly

When using GStreamer, default video configuration is likely to malfunction. Modification of videoconfiguration in main() is necessary to solve the problem. vconf[]=""; Inside the semicolon following should be added
   When Working alone from the webcam input,
vconf[] = "v4l2src device=/dev/video0 use-fixed-fps=false ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24,framerate=5/1 ! identity name=artoolkit sync=true ! fakesink";
   When receiving from UDP connection,
vconf[] = "udpsrc port=5000 ! application/x-rtp, encoding-name=JPEG, payload=96 ! rtpjpegdepay ! jpegdec ! ffmpegcolorspace ! video/x-raw-rgb,bpp=24 ! identity name=artoolkit sync=true ! fakesink";
   When using UDP, open new terminal window for sending side and run,
gst-launch-0.10 -v v4l2src device=/dev/video0 ! ffmpegcolorspace ! video/x-raw-yuv, width=640, height=480, framerate=30/1 ! ffmpegcolorspace ! jpegenc ! rtpjpegpay ! udpsink host=localhost port=5000
Since these are only examples, pipelines should be changed accordingly to environment.



--
EOF
