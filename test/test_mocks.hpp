#pragma once

#include "../src/vc_pipeline/include/processor_base.hpp"

using namespace vc;

class MockIngress : public ProcessorBase {

public:
    explicit MockIngress()
        : ProcessorBase(ProcessorType::Ingress) {
        define_outport("outport");
    }
private:
    void processor_function(ProcessorContext*) override { }
};

class MockTransform : public ProcessorBase {

public:
    explicit MockTransform()
        : ProcessorBase(ProcessorType::Transform) {
        define_inport("inport");
        define_outport("outport");
    }
private:
    void processor_function(ProcessorContext*) override { }

};
