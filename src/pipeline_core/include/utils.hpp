#pragma once
#include <string>
#include "./pipeline_state.hpp"

using std::string;

namespace vc {

    string pipeline_state_to_string(const PipelineState& state) {
        switch(state) {
            case PipelineState::CANCELLED:
                return "CANCELLED";
            case PipelineState::COMPLETED:
                return "COMPLETED";
            case PipelineState::ERROR:
                return "ERROR";
            case PipelineState::IDLE:
                return "IDLE";
            case PipelineState::RUNNING:
                return "RUNNING";
        }
    }

};
