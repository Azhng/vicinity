#include <cassert>
#include <string>
#include "../src/vc_pipeline/include/processor_type.hpp"
#include "../src/vc_pipeline/include/processor_base.hpp" 
#include "../src/vc_pipeline/include/processor_instance.hpp"

using namespace vc;
using namespace std;


class MockProcessor : public ProcessorBase {

public:

    explicit MockProcessor()
        : ProcessorBase(ProcessorType::Ingress) { }

private:

    void processor_function(ProcessorContext* ctx) override { }

};



int main() {

    ProcessorContext dummy_context(nullptr, nullptr);

    ProcessorBase* processor_core = new MockProcessor();
    ProcessorInstance processor(processor_core, nullptr);


    // Check for inport/outport creation

}
