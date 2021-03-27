// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <iomanip>
#include "peopledetect.h"

using namespace cv;
using namespace std;

static const string keys = "{ help h   |   | print help message }"
						   "{ camera c | 0 | capture video from camera (device index starting from 0) }"
						   "{ video v  |   | use video as input }";


int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	parser.about("This sample demonstrates the use of the HoG descriptor.");
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}
	int camera = parser.get<int>("camera");
	string file = parser.get<string>("video");
    //file = "/home/giovanni/Video/jf.mp4";
	cout << "file: " << file << endl;
	if (!parser.check())
	{
		parser.printErrors();
		return 1;
	}

    PeopleDetect pdet(camera, file);

    pdet.onSetup();
    pdet.onConfigure(1);

    RTLIB_ExitCode_t rc;
    do {
        rc = pdet.onRun();
    } while (rc == RTLIB_OK);
	return 0;
}
