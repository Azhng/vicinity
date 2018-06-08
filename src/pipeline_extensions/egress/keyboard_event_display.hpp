#pragma once

#include <map>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/pipeline_signal.hpp"
#include "../../pipeline_core/include/processor_base.hpp"

using std::map;
using std::string;
using std::unique_ptr;
using namespace vc::core;

namespace vc {

namespace extensions {

class KeyboardEventDisplay : public ProcessorBase {

    string window_name;
    map<int, unique_ptr<PipelineSignal>> registered_keyboard_input;

public:

    static const string KEYBOARD_EVENT_DISPLAY_INPORT;

    explicit KeyboardEventDisplay(string);

    void registeredKeyboardInput(int, unique_ptr<PipelineSignal>);

    void run(ProcessorContext*) override;

};

}

};

