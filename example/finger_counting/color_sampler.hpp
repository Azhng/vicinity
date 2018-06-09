#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include "../../src/pipeline_core/include/processor_base.hpp"

using std::string;
using std::vector;

using cv::ColorConversionCodes;
using cv::Point;
using cv::Rect;

using vc::core::ProcessorBase;
using vc::core::ProcessorContext;


class ColorSampler : public ProcessorBase {

    Point center;

    vector<Rect> sampling_boxes;
    vector<Rect> background_sampling_box;

public:

    static const string COLOR_SAMPLER_INPORT;
    static const string COLOR_SAMPLER_OUTPORT;

    static const string SAMPLED_STORE_KEY;
    static const string BG_SAMPLED_STORE_KEY;

    explicit ColorSampler();

    void run(ProcessorContext*) override;

private:

    void _generate_sampling_box();

    void _generate_background_sampling_box();

    vector<int> _extract_mean(const cv::Mat&, const cv::Rect&);

};

