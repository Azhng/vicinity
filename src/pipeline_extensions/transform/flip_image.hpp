#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/processor_base.hpp"

using namespace vc::core;

namespace vc {

namespace extensions {

class FlipImage : public ProcessorBase {

public:

    static const string FLIP_IMAGE_INPORT;
    static const string FLIP_IMAGE_OUTPORT;

    explicit FlipImage();

    void run(ProcessorContext*) override;

};

};

};

