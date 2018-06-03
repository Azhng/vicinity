#pragma once 

#include <memory>
#include <vector>
#include <tuple>
#include <boost/uuid/uuid.hpp>
#include "./pipeline_context.hpp"
#include "./processor_context.hpp"
#include "./processor_base.hpp"

namespace vc {

using boost::uuids::uuid;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::tuple;
using std::make_tuple;


class ProcessorBase;

/**
 * ProcessorInstance: an abstraction of the instantiation of [[ProcessorBase]] 
 *    Containing weak references to the [[PipelineContext]] 
 *    Owner the [[ProcessorBase]] and its corresponding [[ProcessorContext]]
 */
class ProcessorInstance {

    uuid processor_instance_uuid;
    unique_ptr<ProcessorBase> processor;
    unique_ptr<ProcessorContext> processor_context;
    const PipelineContext* pipeline_context;

    vector<tuple<ProcessorInstance*, string>> parent_connections;
    vector<tuple<ProcessorInstance*, string>> child_connections;

public:

    explicit ProcessorInstance(ProcessorBase*, const PipelineContext*);

    void attachChildProcessor(ProcessorInstance* child_processor,
            string child_port_name,
            string parent_port_name) {
        // Phase 1 restriction, only single downstream processor
        child_connections.push_back(make_tuple(child_processor, child_port_name));
        child_processor->parent_connections.push_back(make_tuple(this, parent_port_name));
    }

    void runProcessor();

    const vector<tuple<ProcessorInstance*, string>>& children() const {
        return this->child_connections;
    }

    const vector<tuple<ProcessorInstance*, string>>& parents() const {
        return this->parent_connections;
    }

    ProcessorBase* getProcessorBase() {
        return processor.get();
    }

    ProcessorContext* getProcessorContext() {
        return processor_context.get();
    }

    ~ProcessorInstance();

};

};
