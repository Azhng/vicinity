#include "./color_converter.hpp"
#include "../../pipeline_core/include/processor_context.hpp"
#include "../../pipeline_core/include/processor_type.hpp"
#include <memory>

using namespace vc;
using namespace vc::core;
using namespace vc::extensions;

using std::unique_ptr;
using cv::Mat;
using cv::cvtColor;
using cv::COLOR_BGR2GRAY;

const string ColorConverter::COLOR_CONVERTER_INPORT = "COLOR_CONVERTER_INPORT";
const string ColorConverter::COLOR_CONVERTER_OUTPORT = "COLOR_CONVERTER_OUTPORT";

ColorConverter::ColorConverter(ColorConversionCodes code) :
    ProcessorBase(ProcessorType::Transform),
    conversion_code{code} {

    define_inport(COLOR_CONVERTER_INPORT);
    define_outport(COLOR_CONVERTER_OUTPORT);
}

void ColorConverter::processor_function(ProcessorContext* ctx) {
    unique_ptr<Mat> image = ctx->fromInport(COLOR_CONVERTER_INPORT);
	cv::cvtColor(*image, *image, conversion_code);
    ctx->toOutport(COLOR_CONVERTER_OUTPORT, std::move(image));
}
