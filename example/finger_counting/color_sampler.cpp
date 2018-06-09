#include "./color_sampler.hpp"
#include "../../src/pipeline_extensions/transform/color_converter.hpp"
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

const string ColorSampler::SAMPLED_STORE_KEY = "SAMPLE_VALUES";
const string ColorSampler::BG_SAMPLED_STORE_KEY = "BG_SAMPLED_VALUES";

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

void printVector(const char * str, const vector<int>& s){
    std::cout << str << " " << s[0] << " " << s[1] << " " << s[2] << std::endl;
}

void ColorSampler::run(ProcessorContext* ctx) {
    unique_ptr<Mat> image = ctx->fromInport(COLOR_SAMPLER_INPORT);
    Mat* original_image = ctx->retrieveFromPipeline<Mat>(vc::extensions::ColorConverter::COLOR_CONVERTER_CACHE_KEY);

    unique_ptr<vector<vector<int>>> sampled_values = make_unique<vector<vector<int>>>();
    unique_ptr<vector<vector<int>>> background_values = make_unique<vector<vector<int>>>();

    Size s = image->size();
    center = Point(s.width/2, s.height/2);

    if (sampling_boxes.size() == 0) {
        _generate_sampling_box();
        _generate_background_sampling_box();
    }

    for(const Rect& r : sampling_boxes){
      sampled_values->push_back(_extract_mean(*image, r));
      rectangle(*original_image, r, cv::Scalar(0, 255, 0), 3);
    }
    
    for(const Rect& r : background_sampling_box){
      background_values->push_back(_extract_mean(*image, r));
      rectangle(*original_image, r, cv::Scalar(0, 255, 255), 3);
    }

    ctx->storeToPipeline<vector<vector<int>>>(SAMPLED_STORE_KEY, std::move(sampled_values));
    ctx->storeToPipeline<vector<vector<int>>>(BG_SAMPLED_STORE_KEY, std::move(background_values));

    ctx->toOutport(COLOR_SAMPLER_OUTPORT, std::move(image));
}
