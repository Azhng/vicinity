#pragma once

namespace vc {
    enum processor_type_t {
        Ingress,
        Transforma,
        Egress
    };
    
    using ProcessorType = enum processor_type_t;
};
