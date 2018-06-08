#include <memory>
#include <opencv2/opencv.hpp>
#include "./window_display.hpp"
#include "../../pipeline_core/include/processor_context.hpp"

using namespace vc;
using namespace vc::core;
using namespace vc::extensions;

using cv::Mat;
using cv::namedWindow;
using cv::imshow;
using cv::WINDOW_NORMAL;
using cv::waitKey;
using std::unique_ptr;
using std::make_unique;

const string WindowDisplay::WINDOW_DISPLAY_INPORT = "WINDOW_DISPLAY_INPORT";

WindowDisplay::WindowDisplay(string win_name)
    : ProcessorBase(ProcessorType::Egress, true), window_name(win_name) {
    define_inport(WINDOW_DISPLAY_INPORT);
}

void WindowDisplay::run(ProcessorContext* ctx)  {
    unique_ptr<Mat> image = ctx->fromInport(WINDOW_DISPLAY_INPORT);
    namedWindow(window_name, WINDOW_NORMAL);
    imshow(window_name, *image);
    cv::waitKey(1);
}

