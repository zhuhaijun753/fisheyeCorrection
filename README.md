# fisheyeCorrection
Fisheye correction using distortion table

This is a fisheye camera correction project.
It use distortion table provided by the camera manufactor and undistort the fisheye images to normal pinhole images.

##install
It's a CMAKE project
It's need OpenCV-2.4 and eigen3 library.
I add library link in CMakeLists.txt
Install OpenCV and eigen3 by:

    $sudo apt-get install libopencv-dev libeigen3-dev

##Distortion table:
You can refer to sw4066.txt to find what a distortion table used in this project look like.
distortion table used in this project is defined like this:
It's a table contain 1001 elements which correspond to 0 to 100 degree in 0.1 step size. The degree means the size of the angle between the ray and optical axis of camera.
And the elements int the table means the distance between the CCD center and the point that the ray project in the CCD.

##What dose Distortion table mean?
The order of distortion table correspond the angle between the ray and optical axis of camera. The ray decide a pixel in the normal pinhole image.
The elements in the distortino table correspond the distance between the CCD center and the point that the ray project in the CCD. It's decide a pixel in fisheye image.
So the distortion table tells a correspondens of fisheye pixels and the normal pinhole image.

##How to use it?
You can include tools.h. Then use correctImage and correctVideo function. Notice that you have to give it a correction table file correspond your fisheye camera.

You also can include fisheyeCorrector.h in your project.Then use fisheyeCorrector and it's member function correctImage.

##Parameters of executable file for video

distortion table

path to video file

path to store some corrected frames.

##Parameters of executable file for image set

distortion table

image file list

path to store corrected images
