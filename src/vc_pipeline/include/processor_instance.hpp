#pragma once 

#include <memory>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "./pipeline_context.hpp"
#include "./processor_context.hpp"
#include "./processor_base.hpp"

namespace vc {

using boost::uuids::random_generator;
using boost::uuids::uuid;
using std::unique_ptr;
using std::make_unique;
using std::vector;


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

    vector<ProcessorInstance*> parent_processors;
    vector<ProcessorInstance*> child_processors;

public:

    explicit ProcessorInstance(ProcessorBase* processor,
            const PipelineContext* pipeline_context)
        :  processor_instance_uuid{random_generator()()},
           processor{std::unique_ptr<ProcessorBase>(processor)},
           processor_context{make_unique<ProcessorContext>(this, pipeline_context)} { }


    void attachChildProcessor(ProcessorInstance* child_processor) {
        // Phase 1 restriction, only single downstream processor
        child_processors.push_back(child_processor);
        child_processor->parent_processors.push_back(this);
    }


    const vector<ProcessorInstance*>& children() const {
        return this->child_processors;
    }

    const vector<ProcessorInstance*>& parents() const {
        return this->parent_processors;
    }


    ~ProcessorInstance();


};

};
