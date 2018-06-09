#include <algorithm>
#include <iostream>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "./include/processor_base.hpp"
#include "./include/processor_instance.hpp"
#include "./include/pipeline_context.hpp"

using namespace vc;
using std::get;
using boost::uuids::random_generator;
using boost::uuids::to_string;

core::ProcessorInstance::ProcessorInstance(ProcessorBase* processor,
                                     PipelineContext* pipeline_context)
    : processor_instance_uuid{random_generator()()},
           processor{std::unique_ptr<ProcessorBase>(processor)},
           processor_context{make_unique<core::ProcessorContext>(this, pipeline_context)},
           pipeline_context{pipeline_context} { }

void core::ProcessorInstance::attachChildProcessor(ProcessorInstance* child_processor,
        string child_port_name, string parent_port_name) {
        // Phase 1 restriction, only single downstream processor
    child_connections[child_processor].push_back(make_tuple(parent_port_name, child_port_name));
    child_processor->parent_connections[this].push_back(make_tuple(parent_port_name, child_port_name));
}

void core::ProcessorInstance::removeChildProcessor(ProcessorInstance* child_processor) {
    child_processor->parent_connections.erase(this);
    child_connections.erase(child_processor);
}

void core::ProcessorInstance::disconnectParents() {
    parent_connections.clear();
}

void core::ProcessorInstance::runProcessor() {
    // Phase 1 Implementation
    // TODO: refactor this into middleware stack pattern

    // check if pipeline is being cancelled 
    // PipelineCancelMiddleware
    core::PipelineState current_state = pipeline_context->getPipelineState();
    if ((current_state == core::PipelineState::CANCELLED) || ( current_state == core::PipelineState::ERROR)) {
        return;
    }

    // check if the parent outport is correctly being filled
    // FlowDataFetchingMiddleware
    if (processor->getProcessorType() != core::ProcessorType::Ingress) {
        auto [parent, port_connections] = *parent_connections.begin();
        string parent_port_name = get<0>(port_connections[0]);
        string child_port_name = get<1>(port_connections[0]);

        // if yes, move the data from parent port to current processor port
        // else we reschedule this processor to run again at later date
        // TODO: implement timeout when the pipeline is stuck in BACK_OFF_RESTART
        unique_ptr<Mat> upstream_data = parent->getProcessorContext()->fromOutport(parent_port_name);
        if (upstream_data == nullptr) {
            pipeline_context->submitJob(this);
            processor_context->setProcessorState(ProcessorState::BACK_OFF_RESTART);
            return;
        } else {
            processor_context->toInport(child_port_name, std::move(upstream_data));
        }
    }


    // PostExecutionHookMiddleware
    try {
        processor_context->setProcessorState(ProcessorState::RUNNING);
        processor->run(processor_context.get());
        processor_context->setProcessorState(ProcessorState::COMPLETED);

        // We only submit new job for non-Egress processors
        if (processor->getProcessorType() != ProcessorType::Egress && !child_connections.empty()) {
            auto [child, child_port_connection] = *child_connections.begin();
            child->getProcessorContext()->setProcessorState(ProcessorState::QUEUED);
            pipeline_context->submitJob(child);
        } else {
            // Since in phase 1 we only support single processor chain,
            // it's impossible to have multiple fan-out egress
            // hence we can assume if it is an egress, the pipeline is finished
            // TODO: figure out how to handle fan-out egresses 

            pipeline_context->setPipelineState(PipelineState::COMPLETED);
        }
    } catch (const std::exception& e) {
        pipeline_context->setPipelineState(PipelineState::ERROR);
        processor_context->setProcessorState(ProcessorState::ERROR);
        pipeline_context->setLastErrorMessage("[ERROR]: An error has occur during the pipeline execution with error message: " + string(e.what()));
        std::cout << e.what() << std::endl;
        throw e;
    }

}

core::ProcessorInstance::~ProcessorInstance() {}
