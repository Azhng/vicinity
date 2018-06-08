#include "./include/processor_context.hpp"
#include "./include/processor_instance.hpp"
#include "./include/pipeline_signal.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace vc;


core::ProcessorContext::ProcessorContext(core::ProcessorInstance* processor_instance,
                                         core::PipelineContext* pipeline_context)
        : processor_instance{processor_instance},
          pipeline_context{pipeline_context},
          processor_state{ProcessorState::EMPTY} {
    core::ProcessorBase* processor_base = processor_instance->getProcessorBase();

    const vector<string>& inport_names = processor_base->getInportNames();
    const vector<string>& outport_names = processor_base->getOutportNames();

    // Initializing Processor Port
    switch (processor_base->getProcessorType()) {

        // Phase 1 implementation:
        case core::ProcessorType::Ingress: {
            assert(outport_names.size() != 0);
            outports[outport_names[0]] = nullptr;
            break;
        }

        case core::ProcessorType::Transform: {
            assert(inport_names.size() != 0);
            assert(outport_names.size() != 0);
            inports[inport_names[0]] = nullptr;
            outports[outport_names[0]] = nullptr;
            break;
        }

        case core::ProcessorType::Egress: {
            assert(inport_names.size() != 0);
            inports[inport_names[0]] = nullptr;
            break;
        }
    }

}

unique_ptr<Mat> core::ProcessorContext::fromInport(string port_name) {
    if (inports.find(port_name) == inports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid port: " + port_name);
    }

    return std::move(inports[port_name]);
}

unique_ptr<Mat> core::ProcessorContext::fromOutport(string port_name) {
    if (outports.find(port_name) == outports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid port: " + port_name);
    }

    return std::move(outports[port_name]);
}

void core::ProcessorContext::toInport(string port_name, unique_ptr<Mat> data) {
    if (inports.find(port_name) == inports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid port: " + port_name);
    }

    inports[port_name] = std::move(data);
}

void core::ProcessorContext::toOutport(string port_name, unique_ptr<Mat> data) {
    if (outports.find(port_name) == outports.end()) {
        throw std::runtime_error("[ERROR]: Accessing invalid port: " + port_name);
    }

    outports[port_name] = std::move(data);
}

void core::ProcessorContext::sendSignalToPipeline(PipelineSignal* signal) {
    pipeline_context->sendSignal(signal);
}



