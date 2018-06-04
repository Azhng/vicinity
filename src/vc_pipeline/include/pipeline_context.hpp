#pragma once

#include <map>
#include <string>
#include <queue>
#include <boost/any.hpp>
#include "./pipeline_state.hpp"

namespace vc {

using std::map;
using std::string;
using std::queue;
using boost::any;

class ProcessorInstance;

class PipelineContext {

    PipelineState pipeline_state;
    string last_error_message;
    queue<ProcessorInstance*> job_queue;
    map<string, any> pipeline_cache;

public: 

    explicit PipelineContext() : pipeline_state{PipelineState::IDLE} {}

    PipelineState getPipelineState() const {
        return pipeline_state;
    }

    void setPipelineState(PipelineState newState) {
        pipeline_state = newState;
    }

    const string& getLastErrorMessage() const {
        return last_error_message;
    }

    void setLastErrorMessage(string message) {
        last_error_message = message;
    }

    size_t queueSize() const {
        return job_queue.size();
    }

    void submitJob(ProcessorInstance* job);

    ProcessorInstance* nextJob();
};

};
