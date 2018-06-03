#include "./include/processor_context.hpp"
#include "./include/processor_instance.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

vc::ProcessorContext::ProcessorContext(ProcessorInstance* processor_instance,
                                       const PipelineContext* pipeline_context)
        : processor_instance{processor_instance},
          pipeline_context{pipeline_context} {
    ProcessorBase* processor_base = processor_instance->getProcessorBase();

    const vector<string>& inport_names = processor_base->getInportNames();
    const vector<string>& outport_names = processor_base->getOutportNames();

    // Initializing Processor Port
    switch (processor_base->getProcessorType()) {

        // Phase 1 implementation:
        case ProcessorType::Ingress: {
            assert(outport_names.size() != 0);
            outports[outport_names[0]] = nullptr;
            break;
        }

        case ProcessorType::Transform: {
            assert(inport_names.size() != 0);
            assert(outport_names.size() != 0);
            inports[inport_names[0]] = nullptr;
            outports[outport_names[0]] = nullptr;
            break;
        }

        case ProcessorType::Egress: {
            assert(inport_names.size() != 0);
            inports[inport_names[0]] = nullptr;
            break;
        }
    }

}
