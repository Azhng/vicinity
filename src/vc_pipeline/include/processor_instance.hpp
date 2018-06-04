#pragma once 

#include <memory>
#include <vector>
#include <tuple>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "./pipeline_context.hpp"
#include "./processor_context.hpp"
#include "./processor_base.hpp"

namespace vc {

using boost::uuids::uuid;
using boost::uuids::to_string;
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
    PipelineContext* pipeline_context = nullptr;

    map<ProcessorInstance*, vector<tuple<string, string>>> parent_connections;
    map<ProcessorInstance*, vector<tuple<string, string>>> child_connections;

public:

    explicit ProcessorInstance(ProcessorBase*, PipelineContext*);

    void attachChildProcessor(ProcessorInstance* child_processor,
            string child_port_name,
            string parent_port_name) {
        // Phase 1 restriction, only single downstream processor
        child_connections[child_processor].push_back(make_tuple(parent_port_name, child_port_name));
        child_processor->parent_connections[this].push_back(make_tuple(parent_port_name, child_port_name));
    }

    void runProcessor();

    const map<ProcessorInstance*, vector<tuple<string, string>>> children() const {
        return this->child_connections;
    }

    const map<ProcessorInstance*, vector<tuple<string, string>>> parents() const {
        return this->parent_connections;
    }

    ProcessorBase* getProcessorBase() {
        return processor.get();
    }

    ProcessorContext* getProcessorContext() {
        return processor_context.get();
    }

    string getUUID() const {
        return to_string(processor_instance_uuid);
    }

    ~ProcessorInstance();

};

};
