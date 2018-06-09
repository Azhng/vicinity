#include <memory>
#include <vector>

#include "./contour_detector.hpp"
#include "../../src/pipeline_core/include/vicinity.hpp"

using vc::core::ProcessorType;
using vc::core::ProcessorContext;

const std::string ContourDetector::CONTOUR_DETECTOR_INPORT = "CONTOUR_DETECTOR_INPORT";
const std::string ContourDetector::CONTOUR_DETECTOR_OUTPORT = "CONTOUR_DETECTOR_OUTPORT";

const std::string ContourDetector::CONTOUR_DEFECT_STORE_KEY = "CONTOUR_DEFECT_STORE_KEY";
const std::string ContourDetector::CONTOUR_STORE_KEY = "CONTOUR_STORE_KEY";
const std::string ContourDetector::LARGEST_CONTOUR_STORE_KEY = "LARGEST_CONTOUR_STORE_KEY";

ContourDetector::ContourDetector(std::string _original_image_store_key)
    : ProcessorBase(ProcessorType::Transform),
      original_image_store_key{_original_image_store_key} {

    define_inport(CONTOUR_DETECTOR_INPORT);
    define_outport(CONTOUR_DETECTOR_OUTPORT);
}

void ContourDetector::run(ProcessorContext* ctx) {

    std::unique_ptr<cv::Mat> image = ctx->fromInport(CONTOUR_DETECTOR_INPORT);
    cv::Mat* original_image = ctx->retrieveFromPipeline<cv::Mat>(original_image_store_key);

    std::unique_ptr<std::vector<cv::Mat>> contours = std::make_unique<std::vector<cv::Mat>>();
    std::vector<cv::Vec4i> placeholder;

    // compute max contour

    cv::findContours(*image, *contours, placeholder, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(1, 1));

    std::unique_ptr<std::vector<std::vector<cv::Point>>> contour_vectors = std::make_unique<std::vector<std::vector<cv::Point>>>();

    for(const cv::Mat& m : *contours){
       std::vector<cv::Point> p;
        m.copyTo(p);
        contour_vectors->push_back(p);
    }

    size_t mat_idx = 0;
    double largest_contour_size = -1L;
    for (size_t i = 0; i < contour_vectors->size(); ++i) {
        double contour_area = cv::contourArea((*contour_vectors)[i]);
        if (contour_area > largest_contour_size) {
            largest_contour_size = contour_area;
            mat_idx = i;
        }
    }


    // compute bounding rectangle box
    std::vector<cv::Point> vertices;
    cv::approxPolyDP((*contour_vectors)[mat_idx], vertices, 3, true);
    cv::Rect rectangle = cv::boundingRect(vertices);

    // draw result rectangle
    cv::rectangle(*original_image, cv::Point(rectangle.x, rectangle.y),
                                   cv::Point(rectangle.x + rectangle.width, rectangle.y + rectangle.height),
                                   cv::Scalar(0, 255, 0), 2);

    // detect convex hull and defects
    std::vector<std::vector<cv::Point>> hull_vertices(contour_vectors->size());
    std::vector<std::vector<int>> hull_vertices_indices(contour_vectors->size());
    std::unique_ptr<std::vector<std::vector<cv::Vec4i>>> defects = std::make_unique<std::vector<std::vector<cv::Vec4i>>>(contour_vectors->size());

	for (size_t i = 0; i < contour_vectors->size(); i++) {
		cv::convexHull((*contour_vectors)[i], hull_vertices[i], true);
        cv::convexHull((*contour_vectors)[i], hull_vertices_indices[i], true);
        if (hull_vertices_indices[i].size() > 3){
          cv::convexityDefects((*contour_vectors)[i], hull_vertices_indices[i], (*defects)[i]);
        }
	}

    ctx->storeToPipeline<std::vector<std::vector<cv::Vec4i>>>(CONTOUR_DEFECT_STORE_KEY, std::move(defects));
    ctx->storeToPipeline<std::vector<std::vector<cv::Point>>>(CONTOUR_STORE_KEY, std::move(contour_vectors));
    ctx->storeToPipeline<size_t>(LARGEST_CONTOUR_STORE_KEY, std::make_unique<size_t>(mat_idx));

    cv::drawContours(*original_image, hull_vertices, mat_idx, cv::Scalar(0, 255, 255), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point());


    ctx->toOutport(CONTOUR_DETECTOR_OUTPORT, std::move(image));
}

