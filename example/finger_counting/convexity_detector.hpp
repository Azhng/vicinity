#pragma once

#include <string>
#include "../../src/pipeline_core/include/processor_base.hpp"


class ConvexityDetecter : public vc::core::ProcessorBase {

    std::string original_image_store_key;
    std::string convex_hull_defects_key;

public:

    static const std::string CONVEXITY_DETECTOR_INPORT;
    static const std::string CONVEXITY_DETECTOR_OUTPORT;

    explicit ConvexityDetecter(std::string, std::string);

    void run(vc::core::ProcessorContext*) override;
};

