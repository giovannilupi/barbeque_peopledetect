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

vector<Rect> Detector::detect(InputArray &img)
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

PeopleDetect::PeopleDetect(std::string const &name,
                           std::string const &recipe,
                           RTLIB_Services_t *rtlib,
                           int camera,
                           std::string filename) :
    BbqueEXC(name, recipe, rtlib, RTLIB_LANG_CPP),
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
        return RTLIB_EXC_WORKLOAD_NONE;
    }
    cout << "Press 'q' or <ESC> to quit." << endl;
    cout << "Press <space> to toggle between Default and Daimler detector" << endl;
    return RTLIB_OK;
}

/*!
 * \brief PeopleDetect::onConfigure
 *
 * Called each time BarbequeRTRM assigns a new Application Working Mode
 * (AWM) to the application
 *
 * \param awm_id the ID of the currently assigned AWM
 * \return RTLIB_OK on success
 */
RTLIB_ExitCode_t PeopleDetect::onConfigure(int8_t awm_id)
{
    int32_t proc_nr;    // nr. of CPU cores
    int32_t proc_quota; // CPU quota (e.g. quota = 235% -> 3 CPU cores)
    int32_t acc, gpu;
    int32_t sys,mem;

    if (GetAssignedResources(SYSTEM, sys) != RTLIB_OK)
        cerr << "GetAssignedResources(SYSTEM, sys) failed\n";
    if (GetAssignedResources(PROC_ELEMENT, proc_quota) != RTLIB_OK)
        cerr << "GetAssignedResources(PROC_ELEMENT, proc_quota) failed\n";
    if (GetAssignedResources(PROC_NR, proc_nr) != RTLIB_OK)
        cerr << "GetAssignedResources(PROC_NR, proc_nr) failed\n";
    if (GetAssignedResources(GPU, gpu) != RTLIB_OK)
        cerr << "GetAssignedResources(GPU, gpu) failed\n";
    if (GetAssignedResources(ACCELERATOR, acc) != RTLIB_OK)
        cerr << "GetAssignedResources(ACCELERATOR, acc) failed\n";
    if (GetAssignedResources(MEMORY, mem) != RTLIB_OK)
        cerr << "GetAssignedResources(MEMORY, mem) failed\n";

    cout << "PeopleDetect::onConfigure(): awm id= " << static_cast<int>(awm_id) << endl;
    cout << "PeopleDetect::onConfigure(): system= " << sys << endl;
    cout << "PeopleDetect::onConfigure(): proc_nr= " << proc_nr << endl;
    cout << "PeopleDetect::onConfigure(): proc_quota= " << proc_quota << endl;
    cout << "PeopleDetect::onConfigure(): acc= " << acc << endl;
    cout << "PeopleDetect::onConfigure(): gpu= " << gpu << endl;
    cout << "PeopleDetect::onConfigure(): memory= " << mem << endl;
    return RTLIB_OK;
}

void PeopleDetect::show_frame(vector<Rect> &found, int64 elapsed_ticks)
{
    double elapsed_millis = 1000.0 * (double)elapsed_ticks / getTickFrequency();

    ostringstream buf;
    buf << "Mode: " << detector_.modeName() << " ||| "
        << "MS/FRAME: " << fixed << setprecision(1) << elapsed_millis;
        //<< "FPS: " << fixed << setprecision(1) << (getTickFrequency() / (double)elapsed_ticks);
    putText(frame_, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);

    for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
    {
        Rect &r = *i;
        detector_.adjustRect(r);
        rectangle(frame_, r.tl(), r.br(), cv::Scalar(255, 0, 0), 2);
    }

    imshow("People detector", frame_);
}

RTLIB_ExitCode_t PeopleDetect::onRun()
{
    cap_ >> frame_;
    if (frame_.empty())
    {
        cout << "Finished reading: empty frame" << endl;
        return RTLIB_EXC_WORKLOAD_NONE;
    }
    int64 t = getTickCount();
    vector<Rect> found = detector_.detect(frame_);
    t = getTickCount() - t;
    // show the window
    show_frame(found, t);
    // interact with user

    const char key = (char)waitKey(1);
    if (key == 27 || key == 'q') // ESC
    {
        return RTLIB_EXC_WORKLOAD_NONE;
    }
    else if (key == ' ')
    {
        detector_.toggleMode();
    }
    //cout << "PeopleDetect::onRun(): Hello AEM! cycle="<< Cycles() << endl;
    return RTLIB_OK;
}

RTLIB_ExitCode_t PeopleDetect::onMonitor()
{
    cout << "PeopleDetect::onMonitor(): CPS=" << GetCPS() << endl;
    return RTLIB_OK;
}

RTLIB_ExitCode_t PeopleDetect::onSuspend()
{
    cout << "PeopleDetect::onSuspend(): CPS=" << GetCPS() << endl;
    return RTLIB_OK;
}
