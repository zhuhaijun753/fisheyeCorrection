#include "correctTools.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "A distortion table file path and a video/image file path needed to correct should include in the input parameters." << std::endl;
		exit(0);
	}
	//correctImage(argc, argv);
	correctVideo(argc, argv);
}