#include <memory>
#include <functional>

#include "./sample_thresholding.hpp"
#include "../../src/pipeline_core/include/vicinity.hpp"

using vc::core::ProcessorType;
using vc::core::ProcessorContext;

const std::string SampleThresholding::SAMPLE_THRESHOLDING_INPORT = "SAMPLE_THRESHOLDING_INPORT";
const std::string SampleThresholding::SAMPLE_THRESHOLDING_OUTPORT = "SAMPLE_THRESHOLDING_OUTPORT";


SampleThresholding::SampleThresholding(std::string _sampled_values_key,
                                       std::string _background_sampled_key,
                                       unsigned int _tolerance)
    : ProcessorBase(ProcessorType::Transform),
      sampled_values_key{_sampled_values_key},
      background_sampled_value_key{_background_sampled_key},
      tolerance{_tolerance} {
    define_inport(SAMPLE_THRESHOLDING_INPORT);
    define_outport(SAMPLE_THRESHOLDING_OUTPORT);
}

void printScalar(const char * str, const cv::Scalar& s){
    std::cout << str << " " << s[0] << " " << s[1] << " " << s[2] << std::endl;
}

void SampleThresholding::run(ProcessorContext* ctx) {

    std::unique_ptr<cv::Mat> image = ctx->fromInport(SAMPLE_THRESHOLDING_INPORT);

    // retrieve sampled value from the cache store
    std::vector<std::vector<int>>* sampled_values = ctx->retrieveFromPipeline<std::vector<std::vector<int>>>(sampled_values_key);
    std::vector<cv::Mat> sampled_image_instances;

    for (std::vector<int>& sampled_value : *sampled_values) {
        // compute lower/upper bound we will be using to threshold the image
        cv::Scalar lower_bound = _compute_color_bound(sampled_value, (-1) * tolerance);
        cv::Scalar upper_bound = _compute_color_bound(sampled_value, tolerance);
        
        printScalar("lowerbound", lower_bound);
        printScalar("upperbound", upper_bound);

        cv::Mat thresholded;
        cv::inRange(*image, lower_bound, upper_bound, thresholded);
        sampled_image_instances.push_back(thresholded);
    }

    // create single channel image
    cv::Mat result = cv::Mat::zeros(image->rows, image->cols, CV_8UC1);

    for (cv::Mat& img_sample : sampled_image_instances) {
        cv::bitwise_or(result, img_sample, result);
    }

    // Weird OpenCV bug if i directly do operation on unique_ptr
    result.copyTo(*image);
    cv::GaussianBlur(*image, *image, cv::Size(31, 31), 0.0, 0.0);
    cv::medianBlur(*image, *image, 11);
    cv::erode(*image, *image, cv::Mat());
    cv::dilate(*image, *image, cv::Mat());

    ctx->toOutport(SAMPLE_THRESHOLDING_OUTPORT, std::move(image));
}

cv::Scalar SampleThresholding::_compute_color_bound(std::vector<int>& pixel_mean, int tolerance) {

    std::function<int(int, int)> rebound = [](int value, int bound) -> int {
        if (value > bound) {
            return bound;
        } else if (value < 0) {
            return 0;
        } else {
            return value;
        }
    };

    int H = pixel_mean[0] + (int)(tolerance / 3);
    int S = pixel_mean[1] + tolerance;
    int V = pixel_mean[2] + tolerance;

    return cv::Scalar(rebound(H, 179),
                      rebound(S, 255),
                      rebound(V, 255));
}


