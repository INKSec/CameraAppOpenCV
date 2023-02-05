/*
* C++ 20
# Imporant
# only quit program with ESC!
# Needs Opencv
# 
*/



#include <iostream>
#include <iomanip>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <chrono>
#include <format>


using namespace std;
using namespace cv;



int main()
{
	long counterR = 60000, counterL = 60000; // counter to name pictures
	const int width{ 1280 }, height{ 920 }; // Camera resolution
	const int secondsBetweenPictures = 5;
	const unsigned amountCameras{ 2 };
	time_t start;
	namedWindow("currentCamera");
	start = time(0);
	size_t counterRoundRobin{ 0 };
	std::vector<VideoCapture> videoCaptureContainer;
	std::vector<Mat> imageContainer;
	Mat image;




	for (size_t i{ 0 }; i < amountCameras; i++) {


		VideoCapture cap(i); // opens camera at camera port i
		videoCaptureContainer.push_back(cap);
		videoCaptureContainer[i].set(CAP_PROP_FRAME_WIDTH, width);
		videoCaptureContainer[i].set(CAP_PROP_FRAME_HEIGHT, height);



		imageContainer.push_back(image); // adds a container for each image


		if (!videoCaptureContainer[i].isOpened()) { // error if a port is not available
			cout << "Camera " << i + 1 << " is not working" << endl;
			system("pause");
			return -2;
		}

	}


	while (true) {




		videoCaptureContainer[counterRoundRobin] >> imageContainer[counterRoundRobin];
		//cerr << counterRoundRobin << endl;

		if (imageContainer[counterRoundRobin].empty()) {
			std::cerr << "image container is empty" << std::endl;
			break;
		}


		imshow("currentCamera", imageContainer[counterRoundRobin]); // shows current camera image

		if (time(0) - start == (secondsBetweenPictures)) { // triggers the picture capture loop

			auto givePictureName = [counterRoundRobin, counterL, imageContainer]() {
				
				auto now = std::chrono::system_clock::now(); // get current time
				now += std::chrono::hours(1); // to correct the current time 
				string counterRoundRobinForNamning = "Camera_" + to_string(counterRoundRobin);
				auto timestampFilename = std::format("{:%Y_%m_%d_%H_%M_%OS_}", now) + counterRoundRobinForNamning;

				imwrite("C:/test/" + timestampFilename + ".png", imageContainer[counterRoundRobin]); // save picture at path
			};
			givePictureName();

			counterL++;

			start = start + secondsBetweenPictures; // resets the timer. Needed to take only one picture

			counterRoundRobin++;

			if (counterRoundRobin == amountCameras) {
				counterRoundRobin = 0;
			}


		}


		char c = (char)waitKey(25); //Allowing 25 milliseconds frame processing time and initiating break condition - IMPORTANT DO NOT DELETE
		if (c == 27) { break; } // breaks loop with esc

	}


	for (auto& i : videoCaptureContainer) { // close streams
		i.release();
	}

	return 0;


}

// Datum im Dateinamen von gross bis klein


