#pragma once

#include <string>
#include "../../src/pipeline_core/include/processor_base.hpp"


class ContourDetector : public vc::core::ProcessorBase {

    std::string original_image_store_key;

public:

    static const std::string CONTOUR_DETECTOR_INPORT;
    static const std::string CONTOUR_DETECTOR_OUTPORT;

    explicit ContourDetector(std::string);

    void run(vc::core::ProcessorContext*) override;

};

