#include "./color_sampler.hpp"
#include "../../src/pipeline_core/include/processor_context.hpp"
#include "../../src/pipeline_core/include/processor_type.hpp"
#include <memory>

using namespace vc;
using namespace vc::core;

using std::unique_ptr;

using cv::rectangle;
using cv::Mat;
using cv::Size;
using cv::Scalar;
using cv::COLOR_BGR2GRAY;

const string ColorSampler::COLOR_SAMPLER_INPORT = "COLOR_SAMPLER_INPORT";
const string ColorSampler::COLOR_SAMPLER_OUTPORT = "COLOR_SAMPLER_OUTPORT";

ColorSampler::ColorSampler()
    : ProcessorBase(ProcessorType::Transform) {

    define_inport(COLOR_SAMPLER_INPORT);
    define_outport(COLOR_SAMPLER_OUTPORT);
}

// legacy code from cgn
void ColorSampler::_generate_sampling_box() {
    const int size    = 25;
    const int offsetX = 80;
    const int offsetY = 120;

    sampling_boxes = {
      Rect(center.x - offsetX, center.y - offsetY, size, size),
      Rect(center.x - offsetX, center.y          , size, size),
      Rect(center.x - offsetX, center.y + offsetY, size, size),
      Rect(center.x          , center.y          , size, size),
      Rect(center.x          , center.y - offsetY, size, size),
      Rect(center.x          , center.y + offsetY, size, size),
      Rect(center.x + offsetX, center.y - offsetY, size, size),
      Rect(center.x + offsetX, center.y          , size, size),
      Rect(center.x + offsetX, center.y + offsetY, size, size)
    };
}

// legacy code from cgn
void ColorSampler::_generate_background_sampling_box() {

    const int size    = 25;
    const int offsetX = 400;
    const int offsetY = 270;

    background_sampling_box = {
      Rect(center.x - offsetX, center.y - offsetY, size, size),
      Rect(center.x - offsetX, center.y + offsetY, size, size),
      Rect(center.x - offsetX - 50, center.y          , size, size),
      Rect(center.x + offsetX + 50, center.y          , size, size),
      Rect(center.x + offsetX, center.y - offsetY, size, size),
      Rect(center.x + offsetX, center.y + offsetY, size, size)
    };

}

// legacy code from cgn
vector<int> ColorSampler::_extract_mean(const cv::Mat& image, const cv::Rect& rect) {
  Mat sub_image;
  sub_image = image(rect);
  
  vector<Mat> channels;
  cv::split(sub_image, channels);
  
  vector<int> result = {
    (int)cv::mean(channels[0])[0],
    (int)cv::mean(channels[1])[0],
    (int)cv::mean(channels[2])[0]
  };
  
  return result;
}

void ColorSampler::processor_function(ProcessorContext* ctx) {
    unique_ptr<Mat> image = ctx->fromInport(COLOR_SAMPLER_INPORT);

    unique_ptr<vector<vector<int>>> sampled_values = make_unique<vector<vector<int>>>();
    unique_ptr<vector<vector<int>>> background_values = make_unique<vector<vector<int>>>();

    Size s = image->size();
    center = Point(s.width/2, s.height/2);

    if (sampling_boxes.size() == 0) {
        _generate_sampling_box();
        _generate_background_sampling_box();
    }

    for(const Rect& r : sampling_boxes){
      rectangle(*image, r, cv::Scalar(0, 255, 0), 3);
      sampled_values->push_back(_extract_mean(*image, r));
    }
    
    for(const Rect& r : background_sampling_box){
      rectangle(*image, r, cv::Scalar(0, 255, 255), 3);
      background_values->push_back(_extract_mean(*image, r));
    }

    ctx->storeToPipeline<vector<vector<int>>>("sampled_values", std::move(sampled_values));
    ctx->storeToPipeline<vector<vector<int>>>("background_values", std::move(background_values));

    ctx->toOutport(COLOR_SAMPLER_OUTPORT, std::move(image));
}
