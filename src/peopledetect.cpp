#include "peopledetect.h"

#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

Detector::Detector() : m(Default), hog(), hog_d(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9)
{
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    hog_d.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
}

vector<Rect> Detector::detect(InputArray img)
{
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    vector<Rect> found;
    if (m == Default)
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(), 1.05, 2, false);
    else if (m == Daimler)
        hog_d.detectMultiScale(img, found, 0, Size(8,8), Size(), 1.05, 2, true);
    return found;
}

void Detector::adjustRect(Rect & r) const
{
    // The HOG detector returns slightly larger rectangles than the real objects,
    // so we slightly shrink the rectangles to get a nicer output.
    r.x += cvRound(r.width*0.1);
    r.width = cvRound(r.width*0.8);
    r.y += cvRound(r.height*0.07);
    r.height = cvRound(r.height*0.8);
}

PeopleDetect::PeopleDetect(int camera, std::string filename) :
    camera_(camera),
    filename_(filename)
{
}

/*virtual*/
PeopleDetect::~PeopleDetect()
{

}

RTLIB_ExitCode_t PeopleDetect::onSetup()
{
    if (filename_.empty())
        cap_.open(camera_);
    else
    {
        //file = samples::findFileOrKeep(file);
        cap_.open(filename_);
    }
    if (!cap_.isOpened())
    {
        cout << "Can not open video stream: '"
             << (filename_.empty() ? "<camera>" : filename_)
             << "'" << endl;
        return 2;
    }
    cout << "Press 'q' or <ESC> to quit." << endl;
    cout << "Press <space> to toggle between Default and Daimler detector" << endl;
}

RTLIB_ExitCode_t PeopleDetect::onConfigure(int8_t awm_id)
{

}

RTLIB_ExitCode_t PeopleDetect::onRun()
{
    cap_ >> frame_;
    if (frame_.empty())
    {
        cout << "Finished reading: empty frame" << endl;
        return 2;
    }
    int64 t = getTickCount();
    vector<Rect> found = detector_.detect(frame_);
    t = getTickCount() - t;
    // show the window
    {
        ostringstream buf;
        buf << "Mode: " << detector_.modeName() << " ||| "
            << "FPS: " << fixed << setprecision(1) << (getTickFrequency() / (double)t);
        putText(frame_, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
    }
    for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
    {
        Rect &r = *i;
        detector_.adjustRect(r);
        rectangle(frame_, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    }
    imshow("People detector", frame_);
    // interact with user
    const char key = (char)waitKey(16);
    if (key == 27 || key == 'q') // ESC
    {
        return 2;
    }
    else if (key == ' ')
    {
        detector_.toggleMode();
    }
    return 1;
}

RTLIB_ExitCode_t PeopleDetect::onMonitor()
{

}

RTLIB_ExitCode_t PeopleDetect::onSuspend()
{

}
