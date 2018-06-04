#pragma once

#include <string>
#include <queue>
#include "./pipeline_state.hpp"

namespace vc {

using std::string;
using std::queue;

class ProcessorInstance;

class PipelineContext {

    PipelineState pipeline_state;
    string last_error_message;

    queue<ProcessorInstance*> job_queue;

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

    void submitJob(ProcessorInstance* job) {
        job_queue.push(job);
    }

    ProcessorInstance* nextJob() {
        ProcessorInstance* next_job = job_queue.front();
        job_queue.pop();
        return next_job;
    }

};

};
