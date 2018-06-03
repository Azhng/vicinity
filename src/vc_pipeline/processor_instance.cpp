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


ProcessorInstance::~ProcessorInstance() {}
