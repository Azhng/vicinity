#include <cassert>
#include <string>
#include <opencv2/opencv.hpp>
#include "../src/vc_pipeline/include/processor_type.hpp"
#include "../src/vc_pipeline/include/processor_base.hpp" 
#include "../src/vc_pipeline/include/processor_instance.hpp"
#include "../src/vc_pipeline/include/pipeline_context.hpp"
#include "./test_mocks.hpp"

using namespace vc;


void test_job_queue_submission() {
    PipelineContext pipeline_context =  PipelineContext();
    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, nullptr);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, nullptr);

    pipeline_context.submitJob(ingress_ins);
    pipeline_context.submitJob(transform_ins);

    assert(pipeline_context.nextJob() == ingress_ins);
    assert(pipeline_context.nextJob() == transform_ins);
}

int main() {
    test_job_queue_submission();
}
