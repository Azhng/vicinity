#include <cassert>
#include <string>
#include <iostream>
#include "../src/pipeline_core/include/processor_type.hpp"
#include "../src/pipeline_core/include/processor_base.hpp" 
#include "../src/pipeline_core/include/processor_context.hpp"
#include "../src/pipeline_core/include/processor_instance.hpp"
#include "../src/pipeline_core/include/pipeline_context.hpp"
#include "../src/pipeline_core/include/pipeline.hpp"
#include "./test_mocks.hpp"

using namespace vc;
using namespace vc::core;
using namespace std;

void test_signal_processing() {
    int test_val = 0;
    MockSignal m(&test_val);

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

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

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(ingress_ins));
    pipeline.runPipelineOnce();

    assert(test_val == 0);

    pipeline_ctx->sendSignal(&m);
    pipeline.runPipelineOnce();

    // check if signal handling code is being used
    assert(test_val == 1);

    // check if signal is being cleared
    pipeline.runPipelineOnce();
    assert(test_val == 1);
}

void test_running_pipeline() {
    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

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

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(ingress_ins));
    pipeline.runPipelineOnce();
}

void test_adding_processor_chain() {
    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();
    ProcessorBase* egress = new MockEgress();
    ProcessorBase* rogue = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, pipeline_ctx);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, pipeline_ctx);
    ProcessorInstance* egress_ins = new ProcessorInstance(egress, pipeline_ctx);
    ProcessorInstance* rogue_ins = new ProcessorInstance(rogue, pipeline_ctx);

    ingress_ins->attachChildProcessor(transform_ins,
            MockTransform::MockTransformInportName,
            MockIngress::MockIngressOutportName);

    transform_ins->attachChildProcessor(egress_ins,
            MockEgress::MockEgressInportName,
            MockTransform::MockTransformOutportName);

    unique_ptr<ProcessorInstance> root(ingress_ins);
    pipeline.attachProcessorChain(std::move(root));

    assert(pipeline.getProcessorInstance(ingress_ins->getUUID()) == ingress_ins);
    assert(pipeline.getProcessorInstance(transform_ins->getUUID()) == transform_ins);
    assert(pipeline.getProcessorInstance(egress_ins->getUUID()) == egress_ins);

    pipeline.attachProcessor(unique_ptr<ProcessorInstance>(rogue_ins));
    assert(pipeline.getProcessorInstance(rogue_ins->getUUID()) == rogue_ins);
}

int main() {
    test_adding_processor_chain();
    test_running_pipeline();
    test_signal_processing();
}
