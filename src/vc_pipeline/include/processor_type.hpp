#pragma once

namespace vc {
    enum processor_type_t {
        Ingress,
        Transform,
        Egress
    };
    
    using ProcessorType = enum processor_type_t;
};
