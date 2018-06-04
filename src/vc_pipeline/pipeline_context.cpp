#include "./include/pipeline_context.hpp"

using namespace vc;


void vc::PipelineContext::submitJob(ProcessorInstance* job) {
    job_queue.push(job);
}

ProcessorInstance* vc::PipelineContext::nextJob() {
    ProcessorInstance* next_job = job_queue.front();
    job_queue.pop();
    return next_job;
}
