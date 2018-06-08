#pragma once

#include <string>

#include "../../src/pipeline_core/include/vicinity.hpp"
#include "../../src/pipeline_extensions/egress/keyboard_event_display.hpp"

class SideBySideDisplayer : public vc::extensions::KeyboardEventDisplay {

    std::string other_window_name;
    std::string storage_key;

public:

    explicit SideBySideDisplayer(std::string, std::string, std::string);

    void run(vc::core::ProcessorContext*) override;


};


