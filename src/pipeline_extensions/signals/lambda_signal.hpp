#pragma once 

#include <functional>
#include "../../pipeline_core/include/vicinity.hpp"

using namespace vc::core;
using std::string;
using std::function;

namespace vc {

namespace extensions {

class LambdaSignal : public PipelineSignal {

    Pipeline* pipeline;
    function<void(Pipeline*)> lambda;

public:

    explicit LambdaSignal(Pipeline*, function<void(Pipeline*)>);

private:

    void _process_signal() override;

};

};

};
