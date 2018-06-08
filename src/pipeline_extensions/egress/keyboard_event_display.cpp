#include <memory>
#include <opencv2/opencv.hpp>
#include "./keyboard_event_display.hpp"
#include "../../pipeline_core/include/processor_context.hpp"
#include "../../pipeline_core/include/pipeline_context.hpp"

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

const string KeyboardEventDisplay::KEYBOARD_EVENT_DISPLAY_INPORT = "KEYBOARD_EVENT_DISPLAY_INPORT";

KeyboardEventDisplay::KeyboardEventDisplay(string win_name)
    : ProcessorBase(ProcessorType::Egress, true), window_name(win_name) {
    define_inport(KEYBOARD_EVENT_DISPLAY_INPORT);
}

void KeyboardEventDisplay::registeredKeyboardInput(int key, unique_ptr<PipelineSignal> signal) {
    registered_keyboard_input[key] = std::move(signal);
}

void KeyboardEventDisplay::run(ProcessorContext* ctx)  {

    unique_ptr<Mat> image = ctx->fromInport(KEYBOARD_EVENT_DISPLAY_INPORT);
    namedWindow(window_name, WINDOW_NORMAL);
    imshow(window_name, *image);
    int key = cv::waitKey(1);
    if (registered_keyboard_input.count(key) == 1) {
        ctx->sendSignalToPipeline(registered_keyboard_input[key].get());
    }
}

