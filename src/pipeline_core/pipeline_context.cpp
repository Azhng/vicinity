#include "./include/pipeline_context.hpp"
#include "./include/pipeline_signal.hpp"

using namespace vc;


void core::PipelineContext::submitJob(ProcessorInstance* job) {
    job_queue.push(job);
}

core::ProcessorInstance* core::PipelineContext::nextJob() {
    ProcessorInstance* next_job = job_queue.front();
    job_queue.pop();
    return next_job;
}

core::PipelineContext::~PipelineContext() {
    if (pipeline_signal != nullptr) {
        delete pipeline_signal;
        pipeline_signal = nullptr;
    }
}
