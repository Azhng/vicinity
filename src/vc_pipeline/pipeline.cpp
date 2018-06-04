#include "./include/pipeline.hpp"
#include <boost/asio/thread_pool.hpp>
#include <memory>

using namespace vc;

using std::make_unique;
using boost::asio::thread_pool;

Pipeline::Pipeline(size_t num_of_threads)
    : worker_pool{num_of_threads},
      pipeline_context(make_unique<PipelineContext>()) { }


void Pipeline::attachProcessorChain(unique_ptr<ProcessorInstance> root) {
    for (auto [child_processor, port_connections] : root->children()) {
        attachProcessorChain(unique_ptr<ProcessorInstance>(child_processor));
    }

    processors[root->getUUID()] = std::move(root);
}

void Pipeline::attachProcessor(unique_ptr<ProcessorInstance> processor) {
    processors[processor->getUUID()] = std::move(processor);
}
