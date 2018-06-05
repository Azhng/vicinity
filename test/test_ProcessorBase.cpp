#include <cassert>
#include <string>
#include "../src/pipeline_core/include/processor_type.hpp"
#include "../src/pipeline_core/include/processor_base.hpp" 
#include "../src/pipeline_core/include/processor_context.hpp"

using namespace vc;
using namespace std;


class MockProcessorIngress : public ProcessorBase {

public:

    explicit MockProcessorIngress()
        : ProcessorBase(ProcessorType::Ingress) {

        try {
            define_inport("defining invalid inport");
            assert(false); // this line should not be executed
        } catch (const std::exception& e) {
            assert(string(e.what()) == "[ERROR] Adding inports to an Ingress element");
        }

        define_outport("outport");
    }

private:

    void processor_function(ProcessorContext*) override { }

};

class MockProcessorEgress : public ProcessorBase {

public:

    explicit MockProcessorEgress()
        : ProcessorBase(ProcessorType::Egress) {

        try {
            define_outport("defining invalid inport");
            assert(false); // this line should not be executed
        } catch (const std::exception& e) {
            assert(string(e.what()) == "[ERROR] Adding outports to an Egress element");
        }

        define_inport("inport");
    }

private:

    void processor_function(ProcessorContext*) override { }

};
int main() {

    ProcessorBase* ingress = new MockProcessorIngress();
    ProcessorBase* egress = new MockProcessorEgress();
    delete ingress;
    delete egress;

}
