#include "../src/pipeline_core/include/vicinity.hpp"
#include "../src/pipeline_extensions/ingress/video_fetcher.hpp"
#include "../src/pipeline_extensions/egress/window_display.hpp"
#include "../src/pipeline_extensions/transform/flip_image.hpp"


using std::unique_ptr;
using namespace vc;
using namespace vc::core;
using namespace vc::extensions;


int main() {

    // No need to free the resources as Pipeline takes care of resource management

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* video_fetch = new VideoFetcher();
    ProcessorBase* flip = new FlipImage();
    ProcessorBase* window_output = new WindowDisplay("Simple Video Stream");

    ProcessorInstance* video_fetch_instance = new ProcessorInstance(video_fetch, pipeline_ctx);
    ProcessorInstance* flip_instance = new ProcessorInstance(flip, pipeline_ctx);
    ProcessorInstance* window_output_instance = new ProcessorInstance(window_output, pipeline_ctx);

    video_fetch_instance->attachChildProcessor(flip_instance,
            FlipImage::FLIP_IMAGE_INPORT,
            VideoFetcher::VIDEO_FETCHER_OUTPORT);

    flip_instance->attachChildProcessor(window_output_instance,
            WindowDisplay::WINDOW_DISPLAY_INPORT,
            FlipImage::FLIP_IMAGE_OUTPORT);

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(video_fetch_instance));
    pipeline.runPipeline();
}
