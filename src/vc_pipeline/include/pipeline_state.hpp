#pragma once

namespace vc {
    enum PipelineState {
        RUNNING   = 1,
        CANCELLED = 2,
        ERROR     = 4,
        COMLETED  = 8
    };
};
