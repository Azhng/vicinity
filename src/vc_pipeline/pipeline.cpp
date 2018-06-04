#include "./include/pipeline.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>

using namespace vc;

using std::make_unique;
using boost::asio::thread_pool;
using boost::uuids::uuid;

Pipeline::Pipeline(size_t num_of_threads)
    : worker_pool{num_of_threads},
      pipeline_context(make_unique<PipelineContext>()) { }


void Pipeline::attachProcessorChain(unique_ptr<ProcessorInstance> root) {
    uuid root_uuid = root->getUUID();
    roots.push_back(root_uuid);
    _attach_processors(std::move(root));
}

void Pipeline::attachProcessor(unique_ptr<ProcessorInstance> processor, bool is_root) {
    if (is_root) {
        roots.push_back(processor->getUUID());
    }
    processors[processor->getUUID()] = std::move(processor);
}

void Pipeline::_attach_processors(unique_ptr<ProcessorInstance> root) {
    for (auto [child_processor, port_connections] : root->children()) {
        _attach_processors(unique_ptr<ProcessorInstance>(child_processor));
    }

    processors[root->getUUID()] = std::move(root);
}

void Pipeline::runPipeline() {
    // Phase 1 implementation 

}

