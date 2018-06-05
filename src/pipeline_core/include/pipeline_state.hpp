#pragma once

namespace vc {
namespace core {

enum class PipelineState {
    RUNNING,
    CANCELLED,
    ERROR,
    COMPLETED,
    IDLE
};
};
};
