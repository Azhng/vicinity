#include "../../src/pipeline_core/include/vicinity.hpp"
#include "../../src/pipeline_extensions/ingress/video_fetcher.hpp"
#include "../../src/pipeline_extensions/egress/window_display.hpp"
#include "../../src/pipeline_extensions/transform/flip_image.hpp"
#include "../../src/pipeline_extensions/transform/color_converter.cpp"
#include "./color_sampler.hpp"


using std::unique_ptr;
using cv::COLOR_BGR2HSV;
using namespace vc;
using namespace vc::core;
using namespace vc::extensions;


int main() {

    // No need to free the resources as Pipeline takes care of resource management

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* video_fetch = new VideoFetcher();
    ProcessorBase* flip = new FlipImage();
    ProcessorBase* hsv = new ColorConverter(COLOR_BGR2HSV);
    ProcessorBase* color_sampler = new ColorSampler();
    ProcessorBase* window_output = new WindowDisplay("Finger Counter");

    ProcessorInstance* video_fetch_instance = new ProcessorInstance(video_fetch, pipeline_ctx);
    ProcessorInstance* flip_instance = new ProcessorInstance(flip, pipeline_ctx);
    ProcessorInstance* hsv_instance = new ProcessorInstance(hsv, pipeline_ctx);
    ProcessorInstance* color_sampler_instance = new ProcessorInstance(color_sampler, pipeline_ctx);
    ProcessorInstance* window_output_instance = new ProcessorInstance(window_output, pipeline_ctx);

    video_fetch_instance->attachChildProcessor(flip_instance,
            FlipImage::FLIP_IMAGE_INPORT,
            VideoFetcher::VIDEO_FETCHER_OUTPORT);

    flip_instance->attachChildProcessor(hsv_instance,
            ColorConverter::COLOR_CONVERTER_INPORT,
            FlipImage::FLIP_IMAGE_OUTPORT);

    hsv_instance->attachChildProcessor(color_sampler_instance,
            ColorSampler::COLOR_SAMPLER_INPORT,
            ColorConverter::COLOR_CONVERTER_OUTPORT);

    color_sampler_instance->attachChildProcessor(window_output_instance,
            WindowDisplay::WINDOW_DISPLAY_INPORT,
            ColorSampler::COLOR_SAMPLER_OUTPORT);

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(video_fetch_instance));
    pipeline.runPipeline();
}
