#include <cassert>
#include <string>
#include <iostream>
#include "../src/pipeline_core/include/processor_type.hpp"
#include "../src/pipeline_core/include/processor_base.hpp" 
#include "../src/pipeline_core/include/processor_context.hpp"
#include "../src/pipeline_core/include/processor_instance.hpp"
#include "../src/pipeline_core/include/pipeline_context.hpp"
#include "./test_mocks.hpp"

using namespace vc;
using namespace std;

void test_parent_child_flow_graph() {

    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, nullptr);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, nullptr);

    ingress_ins->attachChildProcessor(transform_ins, "inport", "outport");

    assert(ingress_ins->children().size() == 1);
    auto [child_ptr, dummy] = *ingress_ins->children().begin();
    assert(child_ptr == transform_ins);

    assert(transform_ins->parents().size() == 1);
    auto [parent_ptr, dummy2] = *transform_ins->parents().begin();
    assert(parent_ptr == ingress_ins);

    delete ingress_ins;
    delete transform_ins;
}

void test_removing_child_from_graph() {
    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, nullptr);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, nullptr);

    ingress_ins->attachChildProcessor(transform_ins, "inport", "outport");
    assert(ingress_ins->children().size() == 1);
    assert(transform_ins->parents().size() == 1);
    ingress_ins->removeChildProcessor(transform_ins);
    assert(ingress_ins->children().size() == 0);
    assert(transform_ins->parents().size() == 0);


    delete ingress_ins;
    delete transform_ins;

}

void test_processor_run_state_change() {

    PipelineContext* pipeline_ctx = new PipelineContext();

    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();
    ProcessorBase* egress = new MockEgress();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, pipeline_ctx);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, pipeline_ctx);
    ProcessorInstance* egress_ins = new ProcessorInstance(egress, pipeline_ctx);

    ingress_ins->attachChildProcessor(transform_ins,
            MockTransform::MockTransformInportName,
            MockIngress::MockIngressOutportName);

    transform_ins->attachChildProcessor(egress_ins,
            MockEgress::MockEgressInportName,
            MockTransform::MockTransformOutportName);


    // Checking initial state
    assert(pipeline_ctx->getPipelineState() == PipelineState::IDLE);
    assert(ingress_ins->getProcessorContext()->getProcessorState() == ProcessorState::EMPTY);


    // run the first processor
    ingress_ins->runProcessor();

    assert(ingress_ins->getProcessorContext()->getProcessorState() == ProcessorState::COMPLETED);
    assert(pipeline_ctx->queueSize() == 1);


    // verify new job is being submitted
    ProcessorInstance* should_be_transform_ins = pipeline_ctx->nextJob();
    assert(pipeline_ctx->queueSize() == 0);
    assert(should_be_transform_ins == transform_ins);

    // run second processor
    assert(transform_ins->getProcessorContext()->getProcessorState() == ProcessorState::QUEUED);
    should_be_transform_ins->runProcessor();
    assert(transform_ins->getProcessorContext()->getProcessorState() == ProcessorState::COMPLETED);
    assert(pipeline_ctx->queueSize() == 1);


    // verify job queue status
    ProcessorInstance* should_be_egress = pipeline_ctx->nextJob();
    assert(should_be_egress == egress_ins);
    assert(pipeline_ctx->queueSize() == 0);

    // run the third processor
    assert(egress_ins->getProcessorContext()->getProcessorState() == ProcessorState::QUEUED);
    should_be_egress->runProcessor();
    assert(egress_ins->getProcessorContext()->getProcessorState() == ProcessorState::COMPLETED);
    assert(pipeline_ctx->queueSize() == 0);
    assert(pipeline_ctx->getPipelineState() == PipelineState::COMPLETED);

    delete ingress_ins;
    delete transform_ins;
    delete egress_ins;
    delete pipeline_ctx;
}

class MockFaultyIngress : public ProcessorBase {

public:


    explicit MockFaultyIngress()
        : ProcessorBase(ProcessorType::Ingress) {
        define_outport(MockIngress::MockIngressOutportName);
    }
private:
    void processor_function(ProcessorContext*) override { }
};


void test_faulty_processors() {
    PipelineContext* pipeline_ctx = new PipelineContext();
    ProcessorBase* ingress = new MockFaultyIngress();
    ProcessorBase* transform = new MockTransform();
    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, pipeline_ctx);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, pipeline_ctx);

    ingress_ins->attachChildProcessor(transform_ins,
            MockTransform::MockTransformInportName,
            MockIngress::MockIngressOutportName);

    ingress_ins->runProcessor();
    assert(ingress_ins->getProcessorContext()->getProcessorState() == ProcessorState::COMPLETED);
    assert(transform_ins->getProcessorContext()->getProcessorState() == ProcessorState::QUEUED);
    pipeline_ctx->nextJob()->runProcessor();
    assert(transform_ins->getProcessorContext()->getProcessorState() == ProcessorState::BACK_OFF_RESTART);

    delete ingress_ins;
    delete transform_ins;
    delete pipeline_ctx;
}

class MockExceptionIngress : public ProcessorBase {
public:

    explicit MockExceptionIngress()
        : ProcessorBase(ProcessorType::Ingress) {
        define_outport(MockIngress::MockIngressOutportName);
    }
private:
    void processor_function(ProcessorContext*) override {
        throw std::runtime_error("testing exception");
    }
};


void test_exception_handling() {
    PipelineContext* pipeline_ctx = new PipelineContext();
    ProcessorBase* ingress = new MockExceptionIngress();
    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, pipeline_ctx);


    ingress_ins->runProcessor();
    assert(ingress_ins->getProcessorContext()->getProcessorState() == ProcessorState::ERROR);
    assert(pipeline_ctx->getLastErrorMessage() == "[ERROR]: An error has occur during the pipeline execution with error message: testing exception");


    delete ingress_ins;
    delete pipeline_ctx;

}

int main() {
    test_parent_child_flow_graph();
    test_faulty_processors();
    test_processor_run_state_change();
    test_exception_handling();
    test_removing_child_from_graph();
}

