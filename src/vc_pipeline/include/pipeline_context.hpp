#pragma once

#include <string>
#include "./pipeline_state.hpp"

namespace vc {

using std::string;

class PipelineContext {

    PipelineState pipeline_state;
    string last_error_message;

public: 

    const PipelineState& getPipelineState() const {
        return pipeline_state;
    }

    void setPipelineState(PipelineState newState) {
        pipeline_state = newState;
    }

    const string& getLastErrorMessage() const {
        return last_error_message;
    }

    void setLastErrorMessage(string message) {
        last_error_message = message;
    }

};

};
