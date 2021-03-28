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
#include <memory>
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

    RTLIB_Services_t *rtlib;
    auto ret = RTLIB_Init(basename(argv[0]), &rtlib);
    if (ret != RTLIB_OK) {
        cerr << "ERROR: Unable to init RTLib (Did you start the BarbequeRTRM daemon?)" << endl;
        return RTLIB_ERROR;
    }
    assert(rtlib);

    std::string recipe("peopledetect");
    cout << "INFO: Registering EXC with recipe " << recipe << endl;

    unique_ptr<bbque::rtlib::BbqueEXC> pexc(new PeopleDetect("PeopleDetect", recipe, rtlib, camera, file));

    if (!pexc->isRegistered()) {
        cerr << "ERROR: Register failed (missing the recipe file?)" << endl;
        return RTLIB_ERROR;
    }

    cout << "INFO: Starting EXC control thread " << endl;
    pexc->Start();

    cout << "INFO: Waiting for the EXC termination " << endl;
    pexc->WaitCompletion();

    cout << "INFO: Terminated. " << endl;
    return EXIT_SUCCESS;
}
