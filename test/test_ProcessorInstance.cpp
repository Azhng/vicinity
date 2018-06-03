#include <cassert>
#include <string>
#include <iostream>
#include "../src/vc_pipeline/include/processor_type.hpp"
#include "../src/vc_pipeline/include/processor_base.hpp" 
#include "../src/vc_pipeline/include/processor_context.hpp"
#include "../src/vc_pipeline/include/processor_instance.hpp"
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

int main() {
    test_parent_child_flow_graph();
}

