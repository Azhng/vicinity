#include "./include/pipeline_context.hpp"

using namespace vc;


void core::PipelineContext::submitJob(ProcessorInstance* job) {
    job_queue.push(job);
}

core::ProcessorInstance* core::PipelineContext::nextJob() {
    ProcessorInstance* next_job = job_queue.front();
    job_queue.pop();
    return next_job;
}
