#pragma once

namespace vc {

namespace core {

class PipelineSignal {

public:

    void signalHandler() {
        _process_signal();
    }

    virtual ~PipelineSignal() {}

private:

    virtual void _process_signal() = 0;

};

};

};
