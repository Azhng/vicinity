#include <cassert>
#include <string>
#include <iostream>
#include "../src/vc_pipeline/include/processor_type.hpp"
#include "../src/vc_pipeline/include/processor_base.hpp" 
#include "../src/vc_pipeline/include/processor_context.hpp"
#include "../src/vc_pipeline/include/processor_instance.hpp"
#include "../src/vc_pipeline/include/pipeline_context.hpp"
#include "../src/vc_pipeline/include/pipeline.hpp"
#include "./test_mocks.hpp"

using namespace vc;
using namespace std;

void test_adding_processor_chain() {
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

    unique_ptr<ProcessorInstance> root(ingress_ins);
    pipeline.attachProcessorChain(std::move(root));

    assert(pipeline.getProcessorInstance(ingress_ins->getUUID()) == ingress_ins);
    assert(pipeline.getProcessorInstance(transform_ins->getUUID()) == transform_ins);
    assert(pipeline.getProcessorInstance(egress_ins->getUUID()) == egress_ins);
}

int main() {
    test_adding_processor_chain();
}