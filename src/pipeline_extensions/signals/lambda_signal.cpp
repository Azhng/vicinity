#include "./lambda_signal.hpp"

using namespace vc::extensions;

LambdaSignal::LambdaSignal(Pipeline* _pipeline,
        function<void(Pipeline*)> _lambda)
    : pipeline{_pipeline},
      lambda{_lambda} { }

void LambdaSignal::_process_signal() {
    lambda(pipeline);
}
