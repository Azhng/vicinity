#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/processor_base.hpp"

using namespace vc::core;

namespace vc {

namespace extensions {

using std::string;

class WindowDisplay : public ProcessorBase {

    string window_name;

public:

    static const string WINDOW_DISPLAY_INPORT;

    explicit WindowDisplay(string);

private:

    void processor_function(ProcessorContext*) override;

};

}

};

