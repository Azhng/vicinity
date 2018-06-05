#pragma once 

#include <string>
#include <boost/uuid/uuid.hpp>
#include "../../pipeline_core/include/vicinity.hpp"

using namespace vc::core;
using std::string;
using boost::uuids::uuid;

namespace vc {

namespace extensions {

class LineageModifer : public PipelineSignal {

    Pipeline* pipeline;

    uuid source_processor_uuid;
    uuid old_target_processor_uuid;
    uuid new_target_processor_uuid;

    string source_processor_port_name;
    string new_target_processsor_port_name;

public:

    explicit LineageModifer(Pipeline*,
            uuid src_uuid,
            uuid old_target_uuid,
            uuid new_target_uuid,
            string src_port,
            string new_target_port);

private:

    void _process_signal() override;

};

};

};
