#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "../../src/pipeline_core/include/processor_base.hpp"


class SampleThresholding : public vc::core::ProcessorBase {

    std::string sampled_values_key;
    std::string background_sampled_value_key;

    unsigned int tolerance;

public:

    static const std::string SAMPLE_THRESHOLDING_INPORT;
    static const std::string SAMPLE_THRESHOLDING_OUTPORT;

    explicit SampleThresholding(std::string, std::string, unsigned int);

    void run(vc::core::ProcessorContext*) override;


private:

    cv::Scalar _compute_color_bound(std::vector<int>&, int);

};

