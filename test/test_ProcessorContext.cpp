#include <cassert>
#include <string>
#include <opencv2/opencv.hpp>
#include "../src/pipeline_core/include/processor_type.hpp"
#include "../src/pipeline_core/include/processor_base.hpp" 
#include "../src/pipeline_core/include/processor_instance.hpp"

using namespace vc;
using namespace std;

using cv::Mat;
using cv::NORM_L1;
using cv::Scalar;


bool matrix_equal(Mat lhs, Mat rhs) {
    return !cv::norm(lhs , rhs, NORM_L1);
}

class MockProcessor : public ProcessorBase {

public:

    explicit MockProcessor()
        : ProcessorBase(ProcessorType::Transform) {
        define_inport("inport");
        define_outport("outport");
    }

private:

    void processor_function(ProcessorContext*) override { }

};


void test_should_throw_exception_when_access_non_existent_port() {
    Mat* input_mat = new Mat(2, 2, CV_8UC3, Scalar(1, 2, 3));
    ProcessorBase* processor_core = new MockProcessor();
    ProcessorInstance processor(processor_core, nullptr);

    ProcessorContext* ctx = processor.getProcessorContext();

    try {
        ctx->toInport("dummy", unique_ptr<Mat>(input_mat));
        assert(false); // should never executed 
    } catch (const std::exception& e) {
        assert(string(e.what()) == "[ERROR]: Accessing invalid key");
    }
}

void test_port_read_write_is_working_correctly() {

    Mat* input_mat = new Mat(2, 2, CV_8UC3, Scalar(1, 2, 3));
    ProcessorBase* processor_core = new MockProcessor();
    ProcessorInstance processor(processor_core, nullptr);

    ProcessorContext* ctx = processor.getProcessorContext();

    ctx->toInport("inport", unique_ptr<Mat>(input_mat));
    unique_ptr<Mat> input_result =  ctx->fromInport("inport");

    assert(matrix_equal(*input_result, *input_mat));

    ctx->toOutport("outport", std::move(input_result));
    unique_ptr<Mat> output_result =  ctx->fromOutport("outport");

    assert(matrix_equal(*output_result, *input_mat));
}


void test_if_ports_are_properly_created() {
    ProcessorBase* processor_core = new MockProcessor();
    ProcessorInstance processor(processor_core, nullptr);

    ProcessorContext* ctx = processor.getProcessorContext();

    assert(ctx->fromInport("inport") == nullptr);
    assert(ctx->fromOutport("outport") == nullptr);
}


int main() {

    test_if_ports_are_properly_created();
    test_port_read_write_is_working_correctly();
    test_should_throw_exception_when_access_non_existent_port();

}
