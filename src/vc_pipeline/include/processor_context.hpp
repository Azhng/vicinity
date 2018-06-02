#pragma once

#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include <boost/uuid/uuid.hpp>
#include "./pipeline_context.hpp"
#include "./processor_instance.hpp"

namespace vc {

using cv::Mat;
using std::map;
using std::string;
using boost::uuids::uuid;

class ProcessorContext {

    const ProcessorInstance* processor_instance;
    const PipelineContext* pipeline_context;

    map<string, unique_ptr<Mat>> inports;
    map<string, unique_ptr<Mat>> outports;

public:

    explicit ProcessorContext(const ProcessorInstance* processor_instance, 
            const PipelineContext* pipeline_context)
        : processor_instance{processor_instance},
          pipeline_context{pipeline_context} {}


};

};
