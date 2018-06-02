#pragma once 

#include <memory>
#include <boost/uuid/uuid.hpp>
#include "./processor_base.hpp"

namespace vc {

using boost::uuids::uuid;
using std::unique_ptr;

class ProcessorInstance {

    uuid processor_instance_uuid;
    unique_ptr<ProcessorBase> processor;



};

};
