#pragma once

#include <opencv2/opencv.hpp>
#include "../../pipeline_core/include/processor_base.hpp"

namespace vc {

using cv::VideoCapture;

class VideoFetcher : public ProcessorBase {

    VideoCapture cap;

public:

    static const string VIDEO_FETCHER_OUTPORT;

    explicit VideoFetcher();

    ~VideoFetcher();

private:

    void processor_function(ProcessorContext*) override;

};

};
