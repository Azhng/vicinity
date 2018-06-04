#pragma once

#include <map>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/thread_pool.hpp>
#include "./pipeline_context.hpp"
#include "./processor_instance.hpp"

namespace vc {

using std::map;
using std::unique_ptr;
using boost::uuids::uuid;
using boost::asio::thread_pool;

class Pipeline {

    vector<uuid> roots;
    map<uuid, unique_ptr<ProcessorInstance>> processors;
    thread_pool worker_pool;
    unique_ptr<PipelineContext> pipeline_context;

public:

    explicit Pipeline(size_t);

    /**
     * Given the root (typically ingress) of a chain of processor instances,
     * add all processor instances into the map
     */
    void attachProcessorChain(unique_ptr<ProcessorInstance>);

    /**
     * Attaching a single processor that is not connected to the current pipeline chain
     */
    void attachProcessor(unique_ptr<ProcessorInstance>, bool is_root = false);

    PipelineContext* getPipelineContext() {
        return pipeline_context.get();
    }

    ProcessorInstance* getProcessorInstance(uuid id) {
        return processors[id].get();
    }

    void runPipeline();

private:

    void _attach_processors(unique_ptr<ProcessorInstance>);

    bool _is_pipeline_still_in_runnable_state();

};

};
