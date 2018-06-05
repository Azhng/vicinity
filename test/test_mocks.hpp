#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "../src/pipeline_core/include/processor_base.hpp"
#include "../src/pipeline_core/include/processor_context.hpp"
#include "../src/pipeline_core/include/pipeline_signal.hpp"

using namespace vc;
using namespace vc::core;
using namespace cv;
using namespace std;

class MockSignal : public PipelineSignal {

public:

    int* i;

    MockSignal(int* val) : i{val} {}

private:

    // create side effect so the execution is visible from outside
    void _process_signal() override {
        (*i) += 1;
    }

};

class MockIngress : public ProcessorBase {

public:

    static const string MockIngressOutportName;

    explicit MockIngress()
        : ProcessorBase(ProcessorType::Ingress) {
        define_outport(MockIngressOutportName);
    }
private:
    void processor_function(ProcessorContext* ctx) override {
        cout << "entering mock ingress" << endl;
        unique_ptr<Mat> input_mat = make_unique<Mat>(2, 2, CV_8UC3, Scalar(1, 2, 3));
        ctx->toOutport(MockIngressOutportName, std::move(input_mat));
    }
};

const string MockIngress::MockIngressOutportName = "outport";

class MockTransform : public ProcessorBase {

public:

    static const string MockTransformInportName;
    static const string MockTransformOutportName;

    explicit MockTransform()
        : ProcessorBase(ProcessorType::Transform) {
        define_inport(MockTransformInportName);
        define_outport(MockTransformOutportName);
    }
private:
    void processor_function(ProcessorContext* ctx) override {
        cout << "entering mock transform" << endl;
        unique_ptr<Mat> input_mat = make_unique<Mat>(2, 2, CV_8UC3, Scalar(3, 2, 3));
        ctx->toOutport(MockTransformOutportName, std::move(input_mat));
    }

};

const string MockTransform::MockTransformInportName = "inport";
const string MockTransform::MockTransformOutportName = "outport";

class MockEgress : public ProcessorBase {

public:

    static const string MockEgressInportName;

    explicit MockEgress()
        : ProcessorBase(ProcessorType::Egress) {
        define_inport(MockEgressInportName);
    }
private:
    void processor_function(ProcessorContext*) override {
        cout << "entering mock egress" << endl;
    }
};

const string MockEgress::MockEgressInportName = "inport";

class MockEgressSideEffect : public ProcessorBase {

public:

    int* val;

    static const string MockEgressSideEffectInportName;

    explicit MockEgressSideEffect(int* _val)
        : ProcessorBase(ProcessorType::Egress), val{_val} {
        define_inport(MockEgressSideEffectInportName);
    }
private:
    void processor_function(ProcessorContext*) override {
        (*val) += 56;
    }
};

const string MockEgressSideEffect::MockEgressSideEffectInportName = "inport";
