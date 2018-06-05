#include "../../src/pipeline_core/include/vicinity.hpp"
#include "../../src/pipeline_extensions/ingress/video_fetcher.hpp"
#include "../../src/pipeline_extensions/egress/keyboard_event_display.hpp"
#include "../../src/pipeline_extensions/transform/flip_image.hpp"
#include "../../src/pipeline_extensions/transform/color_converter.hpp"
#include "../../src/pipeline_extensions/signals/lineage_modifier.hpp"
#include "../../src/pipeline_extensions/signals/lambda_signal.hpp"
#include "../../src/pipeline_extensions/signals/signal_stack.hpp"
#include <opencv2/opencv.hpp>

using cv::COLOR_BGR2GRAY;
using cv::COLOR_BGR2HSV;
using boost::uuids::uuid;
using std::unique_ptr;
using namespace vc;
using namespace vc::core;
using namespace vc::extensions;

#include <boost/uuid/uuid_io.hpp>
using boost::uuids::to_string;
using namespace std;


int main() {

    // No need to free the resources as Pipeline takes care of resource management

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* video_fetch = new VideoFetcher();
    ProcessorBase* flip = new FlipImage();
    ProcessorBase* grey_scale = new ColorConverter(COLOR_BGR2GRAY);
    ProcessorBase* hsv = new ColorConverter(COLOR_BGR2HSV);
    KeyboardEventDisplay* window_output = new KeyboardEventDisplay("Hot swap processor");

    ProcessorInstance* video_fetch_instance = new ProcessorInstance(video_fetch, pipeline_ctx);
    ProcessorInstance* flip_instance = new ProcessorInstance(flip, pipeline_ctx);
    ProcessorInstance* grey_scale_instance = new ProcessorInstance(grey_scale, pipeline_ctx);
    ProcessorInstance* hsv_instance = new ProcessorInstance(hsv, pipeline_ctx);
    ProcessorInstance* window_output_instance = new ProcessorInstance(window_output, pipeline_ctx);

    LambdaSignal* disconnect_window_from_grey = new LambdaSignal(&pipeline,
            [window_output_instance](Pipeline* pipeline) -> void {
                ProcessorInstance* window = pipeline->getProcessorInstance(window_output_instance->getUUID());
                window->disconnectParents();
            });

    LineageModifer* add_hsv_to_fip = new LineageModifer(&pipeline,
            flip_instance->getUUID(),
            grey_scale_instance->getUUID(),
            hsv_instance->getUUID(),
            FlipImage::FLIP_IMAGE_OUTPORT,
            ColorConverter::COLOR_CONVERTER_INPORT);

    LineageModifer* add_grey_to_flip = new LineageModifer(&pipeline,
            flip_instance->getUUID(),
            hsv_instance->getUUID(),
            grey_scale_instance->getUUID(),
            FlipImage::FLIP_IMAGE_OUTPORT,
            ColorConverter::COLOR_CONVERTER_INPORT);

    LambdaSignal* add_window_to_hsv = new LambdaSignal(&pipeline,
            [hsv_instance, window_output_instance](Pipeline*) -> void {
                hsv_instance->attachChildProcessor(window_output_instance,
                        KeyboardEventDisplay::KEYBOARD_EVENT_DISPLAY_INPORT,
                        ColorConverter::COLOR_CONVERTER_OUTPORT);
            });

    LambdaSignal* add_window_to_grey = new LambdaSignal(&pipeline,
            [grey_scale_instance, window_output_instance](Pipeline*) -> void {
                grey_scale_instance->attachChildProcessor(window_output_instance,
                        KeyboardEventDisplay::KEYBOARD_EVENT_DISPLAY_INPORT,
                        ColorConverter::COLOR_CONVERTER_OUTPORT);
            });

    SignalStack* switch_to_hsv = new SignalStack(
            unique_ptr<PipelineSignal>(disconnect_window_from_grey),
            make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_hsv_to_fip), 
                make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_window_to_hsv), nullptr)));

    SignalStack* switch_to_grey = new SignalStack(
            unique_ptr<PipelineSignal>(disconnect_window_from_grey),
            make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_grey_to_flip), 
                make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_window_to_grey), nullptr)));

    window_output->registeredKeyboardInput('a', unique_ptr<PipelineSignal>(switch_to_hsv));
    window_output->registeredKeyboardInput('b', unique_ptr<PipelineSignal>(switch_to_grey));

    video_fetch_instance->attachChildProcessor(flip_instance,
            FlipImage::FLIP_IMAGE_INPORT,
            VideoFetcher::VIDEO_FETCHER_OUTPORT);

    flip_instance->attachChildProcessor(grey_scale_instance,
            ColorConverter::COLOR_CONVERTER_INPORT,
            FlipImage::FLIP_IMAGE_OUTPORT);

    grey_scale_instance->attachChildProcessor(window_output_instance,
            KeyboardEventDisplay::KEYBOARD_EVENT_DISPLAY_INPORT,
            ColorConverter::COLOR_CONVERTER_OUTPORT);

    hsv_instance->attachChildProcessor(window_output_instance,
            KeyboardEventDisplay::KEYBOARD_EVENT_DISPLAY_INPORT,
            ColorConverter::COLOR_CONVERTER_OUTPORT);

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(video_fetch_instance));
    pipeline.attachProcessor(unique_ptr<ProcessorInstance>(hsv_instance));
    pipeline.runPipeline();
}
