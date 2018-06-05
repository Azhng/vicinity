#include <memory>
#include <opencv2/opencv.hpp>
#include "./window_output.hpp"
#include "../../pipeline_core/include/processor_context.hpp"

using namespace vc;

using cv::Mat;
using cv::namedWindow;
using cv::imshow;
using cv::WINDOW_NORMAL;
using std::unique_ptr;
using std::make_unique;

const string WindowOutput::WINDOW_INPUT_OUTPORT = "WINDOW_INPUT_OUTPORT";

WindowOutput::WindowOutput(string win_name)
    : ProcessorBase(ProcessorType::Egress), window_name(win_name) {
    define_inport(WINDOW_INPUT_OUTPORT);
}

void WindowOutput::processor_function(ProcessorContext* ctx)  {
    Mat& image = *ctx->fromInport(WINDOW_INPUT_OUTPORT);
    namedWindow(window_name, WINDOW_NORMAL);
    imshow(window_name, image);
}

