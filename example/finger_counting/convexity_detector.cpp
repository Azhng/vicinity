#include <memory>
#include <vector>

#include "./convexity_detector.hpp"
#include "./contour_detector.hpp"
#include "../../src/pipeline_core/include/vicinity.hpp"

using vc::core::ProcessorType;
using vc::core::ProcessorContext;

const std::string ConvexityDetecter::CONVEXITY_DETECTOR_INPORT = "CONVEXITY_DETECTOR_INPORT";
const std::string ConvexityDetecter::CONVEXITY_DETECTOR_OUTPORT = "CONVEXITY_DETECTOR_OUTPORT";

ConvexityDetecter::ConvexityDetecter(std::string _original_image_store_key,
                                     std::string _convex_hull_defects_key)
    : vc::core::ProcessorBase(ProcessorType::Transform),
      original_image_store_key{_original_image_store_key},
      convex_hull_defects_key{_convex_hull_defects_key} {
    define_inport(CONVEXITY_DETECTOR_INPORT);
    define_outport(CONVEXITY_DETECTOR_OUTPORT);
}

void ConvexityDetecter::run(ProcessorContext* ctx) {

    cv::Mat* original_image = ctx->retrieveFromPipeline<cv::Mat>(original_image_store_key);
    size_t* largest_contour_idx = ctx->retrieveFromPipeline<size_t>(ContourDetector::LARGEST_CONTOUR_STORE_KEY);

    std::unique_ptr<cv::Mat> image = ctx->fromInport(CONVEXITY_DETECTOR_INPORT);

    std::vector<std::vector<cv::Point>>& contours = *ctx->retrieveFromPipeline<std::vector<std::vector<cv::Point>>>(ContourDetector::CONTOUR_STORE_KEY);
    std::vector<std::vector<cv::Vec4i>>* defects = ctx->retrieveFromPipeline<std::vector<std::vector<cv::Vec4i>>>(convex_hull_defects_key);

    for(const cv::Vec4i& v  : (*defects)[*largest_contour_idx]){
        float depth = v[3] / 256;
        if(depth > 10){
            int startIdx = v[0];
            cv::Point start(contours[*largest_contour_idx][startIdx]);

            int endIdx   = v[1];
            cv::Point end(contours[*largest_contour_idx][endIdx]);

            int farIdx   = v[2];
            cv::Point far(contours[*largest_contour_idx][farIdx]);
            
            cv::circle(*original_image, start, 10, cv::Scalar(0, 0, 255), 8);

            //int adjSideA = getLengthBetweenPoints(start, far);
            //int adjSideB = getLengthBetweenPoints(end, far);
            //int hypSide  = getLengthBetweenPoints(start, end);
            
            // valid if the angle is smaller than 85 degrees
            //if(inverseCosineLaw(adjSideA, adjSideB, hypSide) < 85){
            //  validTriangleCounts++;
            //}
            
//            cv::line(image, start, end, Scalar(0, 255, 0), 4);
//            cv::line(image, start, far, Scalar(0, 255, 0), 4);
//            cv::line(image, end, far, Scalar(0, 255, 0), 4);
          }
    }


    ctx->toOutport(CONVEXITY_DETECTOR_OUTPORT, std::move(image));
}
