#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "../src/vc_pipeline/include/processor_base.hpp"
#include "../src/vc_pipeline/include/processor_context.hpp"

using namespace vc;
using namespace cv;
using namespace std;

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
