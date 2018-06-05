#include "./lineage_modifier.hpp"

using namespace vc::extensions;

LineageModifer::LineageModifer(Pipeline* _pipeline,
        uuid src_uuid,
        uuid old_target_uuid,
        uuid new_target_uuid,
        string src_port,
        string new_target_port)
    : pipeline{_pipeline},
      source_processor_uuid{src_uuid},
      old_target_processor_uuid{old_target_uuid},
      new_target_processor_uuid{new_target_uuid},
      source_processor_port_name{src_port},
      new_target_processsor_port_name{new_target_port} { }


void LineageModifer::_process_signal() {
    ProcessorInstance* source = pipeline->getProcessorInstance(source_processor_uuid);
    ProcessorInstance* old_target = pipeline->getProcessorInstance(old_target_processor_uuid);
    ProcessorInstance* new_target = pipeline->getProcessorInstance(new_target_processor_uuid);

    source->removeChildProcessor(old_target);
    source->attachChildProcessor(new_target, new_target_processsor_port_name, source_processor_port_name);
}
