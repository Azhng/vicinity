#include <string>
#include <memory>
#include <iostream>
#include "./video_fetcher.hpp"
#include "../../pipeline_core/include/vicinity.hpp"

using namespace std;
using namespace vc;

using std::string;
using std::unique_ptr;
using std::make_unique;

const string VideoFetcher::VIDEO_FETCHER_OUTPORT = "VIDEO_FETCHER_OUTPORT";

VideoFetcher::VideoFetcher()
    : ProcessorBase(ProcessorType::Ingress) {
    define_outport(VIDEO_FETCHER_OUTPORT);

    if (!cap.isOpened()) {
        cap.open(0);
    }

    if (!cap.isOpened()) {
        throw std::runtime_error("[ERROR]: unable to open camera");
    }
}

void VideoFetcher::processor_function(ProcessorContext* ctx)  {
    unique_ptr<Mat> image = make_unique<Mat>();
    bool is_read_successful = cap.read(*image);

    if (is_read_successful) {
        ctx->toOutport(VIDEO_FETCHER_OUTPORT, std::move(image));
    } else {
        throw std::runtime_error("[ERROR]: unable to read image from camera feed");
    }
}


VideoFetcher::~VideoFetcher() {
    cap.release();
}

