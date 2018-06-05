#include <cassert>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include "../src/pipeline_core/include/processor_type.hpp"
#include "../src/pipeline_core/include/processor_base.hpp" 
#include "../src/pipeline_core/include/processor_instance.hpp"
#include "../src/pipeline_core/include/pipeline_context.hpp"
#include "./test_mocks.hpp"

using namespace vc;
using namespace std;


void test_job_queue_submission() {
    PipelineContext pipeline_context =  PipelineContext();
    ProcessorBase* ingress = new MockIngress();
    ProcessorBase* transform = new MockTransform();

    ProcessorInstance* ingress_ins = new ProcessorInstance(ingress, &pipeline_context);
    ProcessorInstance* transform_ins = new ProcessorInstance(transform, &pipeline_context);

    pipeline_context.submitJob(ingress_ins);
    pipeline_context.submitJob(transform_ins);

    assert(pipeline_context.nextJob() == ingress_ins);
    assert(pipeline_context.nextJob() == transform_ins);
}

void test_pipeline_wide_cache() {
    PipelineContext pipeline_context =  PipelineContext();
    unique_ptr<int> i = make_unique<int>(1);
    unique_ptr<char> j = make_unique<char>('a');
    unique_ptr<double> k = make_unique<double>(9.0233);

    pipeline_context.store<int>("i", std::move(i));
    pipeline_context.store<char>("j", std::move(j));
    pipeline_context.store<double>("k", std::move(k));

    assert(*pipeline_context.retrieve<int>("i") == 1);
    assert(*pipeline_context.retrieve<char>("j") == 'a');
    assert(*pipeline_context.retrieve<double>("k") == 9.0233);

}

int main() {
    test_job_queue_submission();
    test_pipeline_wide_cache();
}
