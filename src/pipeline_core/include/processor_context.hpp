#pragma once

#include <map>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <boost/uuid/uuid.hpp>
#include "./pipeline_context.hpp"
#include "./processor_state.hpp"

namespace vc {

namespace core {

using cv::Mat;
using std::map;
using std::string;
using std::unique_ptr;
using boost::uuids::uuid;

class ProcessorInstance;

class PipelineSignal;

class ProcessorContext {

    ProcessorInstance* processor_instance;
    PipelineContext* pipeline_context;

    map<string, unique_ptr<Mat>> inports;
    map<string, unique_ptr<Mat>> outports;

    ProcessorState processor_state;

public:

    explicit ProcessorContext(ProcessorInstance*, PipelineContext*);

    // Phase 1 API:

    void toInport(string, unique_ptr<Mat>);

    unique_ptr<Mat> fromInport(string);

    void toOutport(string, unique_ptr<Mat>);

    unique_ptr<Mat> fromOutport(string);

    const ProcessorState& getProcessorState() const {
        return processor_state;
    }

    void setProcessorState(ProcessorState state) {
        processor_state = state;
    }

    void sendSignalToPipeline(PipelineSignal*);

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

};
