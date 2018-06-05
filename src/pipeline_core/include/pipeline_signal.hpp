#pragma once

namespace vc {

namespace core {

class PipelineSignal {

public:

    void signalHandler() {
        _process_singal();
    }

    virtual ~PipelineSignal() {}

private:

    virtual void _process_singal() = 0;

};

};

};
