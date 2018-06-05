#include "./include/processor_context.hpp"
#include "./include/processor_instance.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


vc::ProcessorContext::ProcessorContext(ProcessorInstance* processor_instance,
                                       const PipelineContext* pipeline_context)
        : processor_instance{processor_instance},
          pipeline_context{pipeline_context},
          processor_state{ProcessorState::EMPTY} {
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

unique_ptr<Mat> vc::ProcessorContext::fromInport(string port_name) {
    if (inports.find(port_name) == inports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid key");
    }

    return std::move(inports[port_name]);
}

unique_ptr<Mat> vc::ProcessorContext::fromOutport(string port_name) {
    if (outports.find(port_name) == outports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid key");
    }

    return std::move(outports[port_name]);
}

void vc::ProcessorContext::toInport(string port_name, unique_ptr<Mat> data) {
    if (inports.find(port_name) == inports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid key");
    }

    inports[port_name] = std::move(data);
}

void vc::ProcessorContext::toOutport(string port_name, unique_ptr<Mat> data) {
    if (outports.find(port_name) == outports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid key");
    }

    outports[port_name] = std::move(data);
}

