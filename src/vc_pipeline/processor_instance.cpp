#include <algorithm>
#include <iostream>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include "./include/processor_base.hpp"
#include "./include/processor_instance.hpp"
#include "./include/pipeline_context.hpp"

using namespace std;
using namespace vc;
using boost::uuids::random_generator;
using boost::lexical_cast;

ProcessorInstance::ProcessorInstance(ProcessorBase* processor,
                                     PipelineContext* pipeline_context)
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
        auto [parent, port_connections] = *parent_connections.begin();
        string parent_port_name = get<0>(port_connections[0]);
        string child_port_name = get<1>(port_connections[0]);


        // if yes, move the data from parent port to current processor port
        // else notify the pipeline that an error has occured 
        if (parent->getProcessorContext()->fromOutport(parent_port_name) == nullptr) {
            pipeline_context->setPipelineState(PipelineState::ERROR);
            //pipeline_context->setLastErrorMessage("[ERROR]: Missing input data from previous pipeline processor at id(processor): " + lexical_cast<string>(processor_instance_uuid));
            return;

        } else {
            unique_ptr<Mat> upstream_data = parent->getProcessorContext()->fromOutport(parent_port_name);
        }
    }

}

ProcessorInstance::~ProcessorInstance() {}
