#include "../../src/pipeline_core/include/vicinity.hpp"
#include "../../src/pipeline_extensions/ingress/video_fetcher.hpp"
#include "../../src/pipeline_extensions/egress/window_display.hpp"
#include "../../src/pipeline_extensions/transform/flip_image.hpp"
#include "../../src/pipeline_extensions/transform/color_converter.cpp"
#include "../../src/pipeline_extensions/signals/lineage_modifier.hpp"
#include "../../src/pipeline_extensions/signals/lambda_signal.hpp"
#include "../../src/pipeline_extensions/signals/signal_stack.hpp"

#include "./color_sampler.hpp"
#include "./side_by_side_displayer.hpp"
#include "./sample_thresholding.hpp"
#include "./contour_detector.hpp"


using std::unique_ptr;
using cv::COLOR_RGB2HSV;
using namespace vc;
using namespace vc::core;
using namespace vc::extensions;


int main() {

    // No need to free the resources as Pipeline takes care of resource management

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* video_fetch = new VideoFetcher();
    ProcessorBase* flip = new FlipImage();
    ProcessorBase* hsv = new ColorConverter(COLOR_RGB2HSV);
    ProcessorBase* color_sampler = new ColorSampler();
    ProcessorBase* sample_thresholding = new SampleThresholding(ColorSampler::SAMPLED_STORE_KEY, ColorSampler::BG_SAMPLED_STORE_KEY, 15);
    ProcessorBase* contour_detector = new ContourDetector(ColorConverter::COLOR_CONVERTER_CACHE_KEY);
    SideBySideDisplayer* window_output = new SideBySideDisplayer("Finger Counter", "Original Image", ColorConverter::COLOR_CONVERTER_CACHE_KEY);

    ProcessorInstance* video_fetch_instance = new ProcessorInstance(video_fetch, pipeline_ctx);
    ProcessorInstance* flip_instance = new ProcessorInstance(flip, pipeline_ctx);
    ProcessorInstance* hsv_instance = new ProcessorInstance(hsv, pipeline_ctx);
    ProcessorInstance* color_sampler_instance = new ProcessorInstance(color_sampler, pipeline_ctx);
    ProcessorInstance* sample_thresholding_instance = new ProcessorInstance(sample_thresholding, pipeline_ctx);
    ProcessorInstance* contour_detector_instance = new ProcessorInstance(contour_detector, pipeline_ctx);
    ProcessorInstance* window_output_instance = new ProcessorInstance(window_output, pipeline_ctx);

    LambdaSignal* disconnect_window_from_upstream = new LambdaSignal(&pipeline,
            [window_output_instance](Pipeline* pipeline) -> void {
                ProcessorInstance* window = pipeline->getProcessorInstance(window_output_instance->getUUID());
                window->disconnectParents();
            });

    LineageModifer* add_thresholder_to_hsv = new LineageModifer(&pipeline,
            hsv_instance->getUUID(),
            color_sampler_instance->getUUID(),
            sample_thresholding_instance->getUUID(),
            ColorConverter::COLOR_CONVERTER_OUTPORT,
            SampleThresholding::SAMPLE_THRESHOLDING_INPORT);

    LineageModifer* add_sampler_to_hsv = new LineageModifer(&pipeline,
            hsv_instance->getUUID(),
            sample_thresholding_instance->getUUID(),
            color_sampler_instance->getUUID(),
            ColorConverter::COLOR_CONVERTER_OUTPORT,
            ColorSampler::COLOR_SAMPLER_INPORT);

    LambdaSignal* add_window_to_thresholder = new LambdaSignal(&pipeline,
            [contour_detector_instance, window_output_instance](Pipeline*) -> void {
                contour_detector_instance->attachChildProcessor(window_output_instance,
                        SideBySideDisplayer::KEYBOARD_EVENT_DISPLAY_INPORT,
                        ContourDetector::CONTOUR_DETECTOR_OUTPORT);
            });

    LambdaSignal* add_window_to_sampler = new LambdaSignal(&pipeline,
            [color_sampler_instance, window_output_instance](Pipeline*) -> void {
                color_sampler_instance->attachChildProcessor(window_output_instance,
                        SideBySideDisplayer::KEYBOARD_EVENT_DISPLAY_INPORT,
                        ColorSampler::COLOR_SAMPLER_OUTPORT);
            });

    SignalStack* switch_to_thresholder = new SignalStack(
            unique_ptr<PipelineSignal>(disconnect_window_from_upstream),
            make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_thresholder_to_hsv), 
                make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_window_to_thresholder), nullptr)));

    SignalStack* switch_to_sampler = new SignalStack(
            unique_ptr<PipelineSignal>(disconnect_window_from_upstream),
            make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_sampler_to_hsv), 
                make_unique<SignalStack>(unique_ptr<PipelineSignal>(add_window_to_sampler), nullptr)));

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
            SideBySideDisplayer::KEYBOARD_EVENT_DISPLAY_INPORT,
            ColorSampler::COLOR_SAMPLER_OUTPORT);

    sample_thresholding_instance->attachChildProcessor(contour_detector_instance,
            ContourDetector::CONTOUR_DETECTOR_INPORT,
            SampleThresholding::SAMPLE_THRESHOLDING_OUTPORT);


    window_output->registeredKeyboardInput('a', unique_ptr<PipelineSignal>(switch_to_thresholder));
    window_output->registeredKeyboardInput('b', unique_ptr<PipelineSignal>(switch_to_sampler));

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(video_fetch_instance));
    pipeline.attachProcessor(unique_ptr<ProcessorInstance>(sample_thresholding_instance));
    pipeline.runPipeline();
}
