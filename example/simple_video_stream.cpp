#include "../src/pipeline_core/include/vicinity.hpp"
#include "../src/pipeline_extensions/ingress/video_fetcher.hpp"
#include "../src/pipeline_extensions/egress/window_display.hpp"


using std::unique_ptr;
using namespace vc;


int main() {

    // No need to free the resources as Pipeline takes care of resource management

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* video_fetch = new VideoFetcher();
    ProcessorBase* window_output = new WindowDisplay("Simple Video Stream");

    ProcessorInstance* video_fetch_instance = new ProcessorInstance(video_fetch, pipeline_ctx);
    ProcessorInstance* window_output_instance = new ProcessorInstance(window_output, pipeline_ctx);

    video_fetch_instance->attachChildProcessor(window_output_instance,
            WindowDisplay::WINDOW_DISPLAY_INPORT,
            VideoFetcher::VIDEO_FETCHER_OUTPORT);

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(video_fetch_instance));
    pipeline.runPipeline();
}