#include "./flip_image.hpp"
#include "../../pipeline_core/include/processor_context.hpp"
#include "../../pipeline_core/include/processor_type.hpp"

using namespace vc;

const string FlipImage::FLIP_IMAGE_INPORT = "FLIP_IMAGE_INPORT";
const string FlipImage::FLIP_IMAGE_OUTPORT = "FLIP_IMAGE_OUTPORT";

FlipImage::FlipImage() :
    ProcessorBase(ProcessorType::Transform) {

    define_inport(FLIP_IMAGE_INPORT);
    define_outport(FLIP_IMAGE_OUTPORT);

}

void FlipImage::processor_function(ProcessorContext* ctx) {
    unique_ptr<Mat> image = ctx->fromInport(FLIP_IMAGE_INPORT);
    cv::flip(*image, *image, 1);
    ctx->toOutport(FLIP_IMAGE_OUTPORT, std::move(image));
}
