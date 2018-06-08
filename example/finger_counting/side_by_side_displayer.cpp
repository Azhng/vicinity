#include <opencv2/opencv.hpp>

#include "./side_by_side_displayer.hpp"

SideBySideDisplayer::SideBySideDisplayer(std::string upstream_window_name,
                                         std::string cached_window_name,
                                         std::string cache_key)
    : vc::extensions::KeyboardEventDisplay(upstream_window_name),
      other_window_name(cached_window_name),
      storage_key(cache_key) { }

void SideBySideDisplayer::run(vc::core::ProcessorContext* ctx) {

    cv::Mat* image = ctx->retrieveFromPipeline<cv::Mat>(storage_key);
    namedWindow(other_window_name, cv::WINDOW_NORMAL);
    imshow(other_window_name, *image);

    vc::extensions::KeyboardEventDisplay::run(ctx);
}
