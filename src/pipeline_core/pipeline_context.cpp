#include "./include/pipeline_context.hpp"
#include "./include/pipeline_signal.hpp"
#include <memory>

using std::atomic;
using std::memory_order_release;
using std::memory_order_acquire;

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

void core::PipelineContext::sendSignal(PipelineSignal* signal) {
    pipeline_signal.store(signal, memory_order_release);
}

core::PipelineSignal* core::PipelineContext::getSignal() const {
    PipelineSignal* signal = pipeline_signal.load(memory_order_acquire);
    return signal;
}

core::PipelineContext::~PipelineContext() {
    PipelineSignal* signal = pipeline_signal.load();
    if (signal != nullptr) {
        delete signal;
        signal = nullptr;
    }
}
