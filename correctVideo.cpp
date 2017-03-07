#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <sstream>
#include "correctTools.h"
#include <sys/types.h>
#include <sys/stat.h>

static void save_frame(const cv::Mat& frame, const int& frame_idx, const std::string& frame_path){
    //According to specific index interval to save frames
    std::stringstream sframe_idx;
    sframe_idx << frame_idx;
    std::string save_path(frame_path + sframe_idx.str() + ".jpg");

    if(!cv::imwrite(save_path, frame)){
        std::cerr << "Something bad occur while saving frame" << frame_idx << std::endl;
        std::cerr << "Check if your folder existed: " << save_path << std::endl;
    }
}

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
	FisheyeCorrector corrector(correction_table,
                               capture.get(CV_CAP_PROP_FRAME_HEIGHT),
                               capture.get(CV_CAP_PROP_FRAME_WIDTH),
                               pixel_height,
                               f_image_,
                               55,
                               55);
	corrector.setAxisDirection(35,35,-7);//30,35,-7
	corrector.updateMap();
    cv::Rect region(cv::Rect(cv::Point(0, 430),
                             cv::Point(corrector.getCorrectedSize().width,
                                       corrector.getCorrectedSize().height - 40)));
	corrector.setClipRegion(region);
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

	int file_name_pos = videoName.find_last_of('/');
	if (file_name_pos != -1)
	{
		correct_file_name += videoName.substr(file_name_pos+1);
		filePath += videoName.substr(0, file_name_pos + 1);
	}
	else
		correct_file_name += videoName;
    //Get the name of this video file
    int perfix_pos = videoName.find_last_of('.');
    std::string pure_name;
    std::string frame_path = argv[3];
    if (perfix_pos != -1){
        pure_name = videoName.substr(file_name_pos+1, perfix_pos - file_name_pos - 1);
        //Create a directory to store frames with right 755
        if(mkdir(frame_path.c_str(),S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0){
            std::cerr << "Can not create at location: " << frame_path << std::endl;
        }else{
            std::cout << "Successfully create floder: " << frame_path << std::endl;
        }
        frame_path += ("/" + pure_name + "/");
        //Create a directory to store frames with right 755
        if(mkdir(frame_path.c_str(),S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0){
            std::cerr << "Can not create at location: " << frame_path << std::endl;
        }else{
            std::cout << "Successfully create floder: " << frame_path << std::endl;
        }
    }

    //Index interval for storing frames
    int interval = 5;

	cv::VideoWriter writer(filePath + correct_file_name,
                           capture.get(CV_CAP_PROP_FOURCC),
                           capture.get(CV_CAP_PROP_FPS),
                           corrector.getCorrectedSize());

	std::cout << "save file to " << filePath + correct_file_name<<std::endl;
	int frameCount = 0;
	while (!frame.empty())
	{
		corrector.correct(frame, corrected_frame);

		cv::imshow("frame", frame);
		cv::imshow("corrected", corrected_frame);

		writer << corrected_frame;
        if(frameCount % interval == 0){
            save_frame(corrected_frame, frameCount, frame_path);
        }
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

