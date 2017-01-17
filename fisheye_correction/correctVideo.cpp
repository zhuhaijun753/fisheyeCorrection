#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "fisheye_corrector\fisheye_corrector.h"
#include "correctTools.h"

void correctVideo(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "need correction table file and video name for parameters" << std::endl;
	}
		

	std::string correction_table = argv[1];
	std::string videoName = argv[2];
	cv::VideoCapture capture(videoName);
	cv::namedWindow("frame", 0);
	cv::namedWindow("corrected", 0);


	cv::Mat frame,corrected_frame;

	if (!capture.isOpened())
	{
		std::cout << "Fail to open video" << std::endl;
		system("pause");
		exit(-1);
	}
	capture >> frame;
	float pixel_height = 0.0042;
	float f_image_ = 306.605;
	FisheyeCorrector corrector(correction_table, capture.get(CV_CAP_PROP_FRAME_HEIGHT), capture.get(CV_CAP_PROP_FRAME_WIDTH), pixel_height, f_image_, 60, 60);
	corrector.setAxisDirection(0,0,0);//30,35,-7
	corrector.updateMap();
	//corrector.setClipRegion(cv::Rect(cv::Point(0, 430), cv::Point(corrector.getCorrectedSize().width-0, corrector.getCorrectedSize().height)));
	//corrector.setSizeScale(0.5);

	cv::Mat K = corrector.getIntrinsicMatrix();
	std::cout << "K" << std::endl << K << std::endl;
	std::ofstream K_file("K.txt");
	K_file << K << std::endl;
	K_file.close();
	//Use this function if you want to find perspective distortion yourself
	//findPerspectiveDistortion(frame, corrector);



	std::string filePath;
	std::string correct_file_name = "correct_";
	int file_name_pos = videoName.find_last_of('\\');
	if (file_name_pos != -1)
	{
		correct_file_name += videoName.substr(file_name_pos+1);
		filePath += videoName.substr(0, file_name_pos + 1);
	}
	else
		correct_file_name += videoName;


	cv::VideoWriter writer(filePath + correct_file_name, capture.get(CV_CAP_PROP_FOURCC), capture.get(CV_CAP_PROP_FPS), corrector.getCorrectedSize());
	std::cout << "save file to " << filePath + correct_file_name<<std::endl;
	int frameCount = 0;
	while (!frame.empty())
	{
		corrector.correct(frame, corrected_frame);

		cv::imshow("frame", frame);
		cv::imshow("corrected", corrected_frame);

		writer << corrected_frame;
		capture >> frame;
		/*if (frameCount > 3000)
			break;*/
		cv::waitKey(10);
		if (frameCount % 300 == 0)
			std::cout << "frame " << frameCount << std::endl;
		frameCount++;
	}
	writer.release();
}

