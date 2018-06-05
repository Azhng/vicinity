#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/processor_base.hpp"

namespace vc {

using std::string;

class WindowOutput : public ProcessorBase {

    string window_name;

public:

    static const string WINDOW_INPUT_OUTPORT;

    explicit WindowOutput(string);

private:

    void processor_function(ProcessorContext*) override;

};

};

