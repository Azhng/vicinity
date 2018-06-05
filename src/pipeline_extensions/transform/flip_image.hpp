#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/processor_base.hpp"

namespace vc {

class FlipImage : public ProcessorBase {

public:

    static const string FLIP_IMAGE_INPORT;
    static const string FLIP_IMAGE_OUTPORT;

    explicit FlipImage();

private:

    void processor_function(ProcessorContext*) override;

};

};

