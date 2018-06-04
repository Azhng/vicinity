#pragma once

namespace vc {

enum class ProcessorState {
    QUEUED,
    RUNNING,
    ERROR,
    COMPLETED,
    EMPTY
};

}
