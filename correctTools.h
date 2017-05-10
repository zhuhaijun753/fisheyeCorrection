#ifndef CORRECT_TOOLS_H
#define CORRECT_TOOLS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/contrib/retina.hpp>
#include "fisheye_corrector/fisheye_corrector.h"


void correctVideo(int argc, char* argv[]);
void correctImage(int argc, char* argv[]);
void correctImages(int argc, char* argv[]);

#endif //CORRECT_TOOLS_H