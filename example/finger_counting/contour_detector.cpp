#include <memory>
#include <vector>

#include "./contour_detector.hpp"
#include "../../src/pipeline_core/include/vicinity.hpp"

using vc::core::ProcessorType;
using vc::core::ProcessorContext;

const std::string ContourDetector::CONTOUR_DETECTOR_INPORT = "CONTOUR_DETECTOR_INPORT";
const std::string ContourDetector::CONTOUR_DETECTOR_OUTPORT = "CONTOUR_DETECTOR_OUTPORT";

ContourDetector::ContourDetector(std::string _original_image_store_key)
    : ProcessorBase(ProcessorType::Transform),
      original_image_store_key{_original_image_store_key} {

    define_inport(CONTOUR_DETECTOR_INPORT);
    define_outport(CONTOUR_DETECTOR_OUTPORT);
}

void ContourDetector::run(ProcessorContext* ctx) {

    std::unique_ptr<cv::Mat> image = ctx->fromInport(CONTOUR_DETECTOR_INPORT);
    cv::Mat* original_image = ctx->retrieveFromPipeline<cv::Mat>(original_image_store_key);

    std::vector<cv::Mat> contours;
    std::vector<cv::Vec4i> placeholder;

    // compute max contour

    cv::findContours(*image, contours, placeholder, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(1, 1));

    size_t mat_idx = 0;
    double largest_contour_size = -1L;
    for (size_t i = 0; i < contours.size(); ++i) {
        double contour_area = cv::contourArea(contours[i]);
        if (contour_area > largest_contour_size) {
            largest_contour_size = contour_area;
            mat_idx = i;
        }
    }


    // compute bounding rectangle box
    std::vector<cv::Point> vertices;
    cv::approxPolyDP(contours[mat_idx], vertices, 3, true);
    cv::Rect rectangle = cv::boundingRect(vertices);

    // draw result rectangle
    cv::rectangle(*original_image, cv::Point(rectangle.x, rectangle.y),
                                   cv::Point(rectangle.x + rectangle.width, rectangle.y + rectangle.height),
                                   cv::Scalar(0, 255, 0), 2);

    // detect convect hull
    std::vector<std::vector<cv::Point>> hull_vertices(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], hull_vertices[i], false);
	}

    cv::drawContours(*original_image, hull_vertices, mat_idx, cv::Scalar(0, 255, 255), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point());

    ctx->toOutport(CONTOUR_DETECTOR_OUTPORT, std::move(image));
}

