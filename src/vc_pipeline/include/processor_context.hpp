#pragma once

#include <map>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <boost/uuid/uuid.hpp>
#include "./pipeline_context.hpp"

namespace vc {

using cv::Mat;
using std::map;
using std::string;
using std::unique_ptr;
using boost::uuids::uuid;

class ProcessorInstance;

class ProcessorContext {

    const ProcessorInstance* processor_instance;
    const PipelineContext* pipeline_context;

    map<string, unique_ptr<Mat>> inports;
    map<string, unique_ptr<Mat>> outports;

public:

    explicit ProcessorContext(ProcessorInstance*, const PipelineContext*);

    // Phase 1 API:

    void toInport(string, unique_ptr<Mat>);

    unique_ptr<Mat> fromInport(string);

    void toOutport(string, unique_ptr<Mat>);

    unique_ptr<Mat> fromOutport(string);

    // Phase 2 API:

    //template <typename T>
    //void toInport(string, unique_ptr<T>);

    //template <typename T>
    //unique_ptr<T> fromInport(string);

    //template <typename T>
    //void toOutport(string, unique_ptr<T>);

    //template <typename T>
    //unique_ptr<T> fromOutport(string);


};

};
