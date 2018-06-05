#pragma once 

#include <string>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include "../../pipeline_core/include/vicinity.hpp"

using namespace vc::core;
using std::string;
using std::unique_ptr;

namespace vc {

namespace extensions {

class SignalStack : public PipelineSignal {

    unique_ptr<PipelineSignal> signal;
    unique_ptr<SignalStack> next_signal;

public:

    explicit SignalStack(unique_ptr<PipelineSignal>, unique_ptr<SignalStack> next = nullptr);

private:

    void _process_signal() override;

};

};

};
