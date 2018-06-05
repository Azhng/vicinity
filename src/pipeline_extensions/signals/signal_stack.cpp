#include "./signal_stack.hpp"
#include "../../pipeline_core/include/pipeline_signal.hpp"

using namespace vc::core;
using namespace vc::extensions;

SignalStack::SignalStack(unique_ptr<PipelineSignal> _current, unique_ptr<SignalStack> _next)
    : signal{std::move(_current)},
      next_signal{std::move(_next)} { }

void SignalStack::_process_signal() {
    signal->signalHandler();
    if (next_signal != nullptr) {
        next_signal->signalHandler();
    }
}
