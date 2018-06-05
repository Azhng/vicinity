#include "./include/pipeline_context.hpp"
#include "./include/pipeline_signal.hpp"
#include <memory>

using std::atomic;

using namespace vc;

core::PipelineContext::PipelineContext()
    : pipeline_state{PipelineState::IDLE},
      pipeline_signal{nullptr} {}

void core::PipelineContext::submitJob(ProcessorInstance* job) {
    job_queue.push(job);
}

core::ProcessorInstance* core::PipelineContext::nextJob() {
    ProcessorInstance* next_job = job_queue.front();
    job_queue.pop();
    return next_job;
}

core::PipelineContext::~PipelineContext() {
    PipelineSignal* signal = pipeline_signal.load();
    if (signal != nullptr) {
        delete signal;
        signal = nullptr;
    }
}
