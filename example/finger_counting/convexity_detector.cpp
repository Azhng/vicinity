#include <memory>
#include <vector>
#include <functional>
#include <cmath>

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

    int valid_triangle_counts = 0;
    for (const cv::Vec4i& v  : (*defects)[*largest_contour_idx]){
        float depth = v[3] / 256;
        if (depth > 10){
            int startIdx = v[0];
            cv::Point start(contours[*largest_contour_idx][startIdx]);

            int endIdx   = v[1];
            cv::Point end(contours[*largest_contour_idx][endIdx]);

            int farIdx   = v[2];
            cv::Point far(contours[*largest_contour_idx][farIdx]);

            cv::circle(*original_image, start, 10, cv::Scalar(0, 0, 255), 8);

            std::function<int(cv::Point, cv::Point)> points_length = [](cv::Point p1, cv::Point p2) -> int {
                int diff_lengthX = abs(p1.x - p2.x);
                int diff_lengthY = abs(p1.y - p2.y);

                return (int)sqrt(pow(diff_lengthX, 2) + pow(diff_lengthY, 2));
            };
            std::function<int(int, int, int)> inverse_cos = [](int a, int b, int c) -> int{
                double ratio = (pow(a,2) + pow(b, 2) - pow(c, 2))/(2 * a * b);
                return (int)(acos(ratio) * 180.0 / M_PI);
            };

            int adjSideA = points_length(start, far);
            int adjSideB = points_length(end, far);
            int hypSide  = points_length(start, end);

            // valid if the angle is smaller than 85 degrees
            if (inverse_cos(adjSideA, adjSideB, hypSide) < 85){
                valid_triangle_counts++;
            }
            
            cv::line(*original_image, start, end, cv::Scalar(255, 255, 0), 4);
            cv::line(*original_image, start, far, cv::Scalar(255, 255, 0), 4);
            cv::line(*original_image, end, far, cv::Scalar(255, 255, 0), 4);
          }
    }

    // since we are counting triangles, number of fingers is valid triangle + 1
    valid_triangle_counts += 1;

    if (valid_triangle_counts > 5){
       valid_triangle_counts = 5;
    }
      
    std::string message = std::to_string(valid_triangle_counts);
      
    cv::putText(*original_image, std::to_string(valid_triangle_counts), cv::Point(80,80), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(0, 255, 255), 5);

    ctx->toOutport(CONVEXITY_DETECTOR_OUTPORT, std::move(image));
}
