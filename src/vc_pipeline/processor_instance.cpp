#include <algorithm>
#include <iostream>
#include <boost/uuid/uuid_generators.hpp>
#include "./include/processor_base.hpp"
#include "./include/processor_instance.hpp"
#include "./include/pipeline_context.hpp"

using namespace std;
using namespace vc;
using boost::uuids::random_generator;

ProcessorInstance::ProcessorInstance(ProcessorBase* processor,
                  const PipelineContext* pipeline_context)
    : processor_instance_uuid{random_generator()()},
           processor{std::unique_ptr<ProcessorBase>(processor)},
           processor_context{make_unique<ProcessorContext>(this, pipeline_context)} { }


void ProcessorInstance::runProcessor() {
    // Phase 1 Implementation

    // check if pipeline is being cancelled 
    if (pipeline_context->getPipelineState() == (PipelineState::CANCELLED | PipelineState::ERROR)) {
        return;
    }

    // check if the parent outport is correctly being filled
    if (processor->getProcessorType() != ProcessorType::Ingress) {
        ProcessorInstance* parent = get<0>(parent_connections[0]);
    }

}

ProcessorInstance::~ProcessorInstance() {}
