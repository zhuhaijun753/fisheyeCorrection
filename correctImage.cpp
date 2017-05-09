#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "correctTools.h"

void correctImage(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "need correction table file and video name for parameters" << std::endl;
	}


	std::string correction_table = argv[1];
	std::string imageName = "./0.png";
	
	cv::Mat image = cv::imread(imageName);
	cv::Mat corrected_frame;

	cv::namedWindow("frame", 0);
	cv::namedWindow("corrected", 0);


	std::string filePath;
	std::string correct_file_name = "correct2_";
	int file_name_pos = imageName.find_last_of('/');
	if (file_name_pos != -1)
	{
		correct_file_name += imageName.substr(file_name_pos + 1);
		filePath += imageName.substr(0, file_name_pos+1);
	}
	else
		correct_file_name += imageName;

	float pixelHeight = 0.0042;
	float f_image_ = 306.605;
	FisheyeCorrector corrector(correction_table, image.rows, image.cols, pixelHeight, f_image_, 55, 55);
	//Deal with the perspective distortion
	//This is a perspective distortion correction matrix finded manually. 
	//Please notice that this is for vertical range at 53 degree  and horiental range 70 degree and different range should use different perspective correction
	cv::Mat perspectiveTransform = (cv::Mat_<double>(3, 3) <<
		0.7120342503081317, -0.9080554014444526, 98.72580114592542,
		-0.03621256531992459, 0.7347221335742586, 15.81086229667521,
		-0.0001161970505587349, -0.002209380538794291, 1
		);
	/*corrector.setPerspectiveTransformation(perspectiveTransform);*/

	//Use this function if you want to find perspective distortion yourself
	//findPerspectiveDistortion(frame, corrector);

	
	corrector.correct(image, corrected_frame);


	std::ofstream K_file("./K.txt");
	K_file << corrector.getIntrinsicMatrix() << std::endl;
	K_file.close();


	cv::imshow("frame", image);
	cv::imshow("corrected", corrected_frame);
	cv::imwrite(filePath+correct_file_name, corrected_frame);
	std::cout << "save file to " << filePath + correct_file_name << std::endl;
	cv::waitKey(0);
}

void correctImages(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "need correction table file and video name for parameters" << std::endl;
	}
	std::string correction_table = argv[1];
	float pixelHeight = 0.0042;
	float f_image_ = 306.605;
	FisheyeCorrector corrector(correction_table, 720, 1280, pixelHeight, f_image_, 55, 55);
	//Left: -35, 35, 7
	//Right: 35, 35, -7
	corrector.setAxisDirection(0,35,0);//30,35,-7
	corrector.updateMap();
	//    cv::Rect region(cv::Rect(cv::Point(0, 430),
	//                             cv::Point(corrector.getCorrectedSize().width,
	//                                       corrector.getCorrectedSize().height)));
	//	corrector.setClipRegion(region);
	//  corrector.setSizeScale(0.5);
	std::string image_list = argv[2];

	std::cout << "read image_list list" << std::endl;
	std::ifstream file(image_list);
	if (!file.is_open())
	{
		std::cout << "open file error";
		exit(-1);
	}
	std::string dst_dir = argv[2];
	if(mkdir(dst_dir.c_str(),S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0){
		std::cerr << "Can not create at location: " << dst_dir << std::endl;
	}else{
		std::cout << "Successfully create floder: " << dst_dir << std::endl;
	}

	std::string imageName;
	while(file >> imageName){
		cv::Mat image = cv::imread(imageName);
		cv::Mat corrected_frame;
		std::string filePath;
		std::string correct_file_name = "correct_";
		int file_name_pos = imageName.find_last_of('/');
		if (file_name_pos != -1){
			correct_file_name += imageName.substr(file_name_pos + 1);
			filePath += imageName.substr(0, file_name_pos+1);
		}
		else{
			correct_file_name += imageName;
		}
		corrector.correct(image, corrected_frame);

		cv::imwrite(dst_dir+"/"+correct_file_name, corrected_frame);
		std::cout << "save file to " << dst_dir + "/" + correct_file_name << std::endl;
	}
	std::cout << "Correct process finished" << std::endl;
	file.close();
}
