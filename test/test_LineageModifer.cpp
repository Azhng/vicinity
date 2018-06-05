#include "../src/pipeline_core/include/vicinity.hpp"
#include "../src/pipeline_extensions/signals/lineage_modifier.hpp"
#include "./test_mocks.hpp"
#include <cassert>
#include <string>
#include <iostream>

using namespace vc;
using namespace vc::core;
using namespace vc::extensions;
using namespace std;

void test_lineage_graph_change() {
    int test_val = 0;

    Pipeline pipeline(4);
    PipelineContext* pipeline_ctx = pipeline.getPipelineContext();

    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* egrees1  = new MockEgress();
    ProcessorBase* egress2 = new MockEgressSideEffect(&test_val);

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, pipeline_ctx);
    ProcessorInstance* egress_ins1 = new ProcessorInstance(egrees1, pipeline_ctx);
    ProcessorInstance* egress_ins2 = new ProcessorInstance(egress2, pipeline_ctx);

    LineageModifer lineage_modify_signal = LineageModifer(&pipeline,
            ingress_ins->getUUID(),
            egress_ins1->getUUID(),
            egress_ins2->getUUID(),
            MockIngress::MockIngressOutportName,
            MockEgressSideEffect::MockEgressSideEffectInportName);

    ingress_ins->attachChildProcessor(egress_ins1,
            MockTransform::MockTransformInportName,
            MockIngress::MockIngressOutportName);

    pipeline.attachProcessorChain(unique_ptr<ProcessorInstance>(ingress_ins));
    pipeline.attachProcessor(unique_ptr<ProcessorInstance>(egress_ins2));
    pipeline.runPipelineOnce();

    assert(test_val == 0);

    pipeline_ctx->sendSignal(&lineage_modify_signal);
    pipeline.runPipelineOnce();

    assert(test_val == 56);
}

int main() {
    test_lineage_graph_change();
}
