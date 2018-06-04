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

    PipelineContext* getPipelineContext() {
        return pipeline_context.get();
    }

    ProcessorInstance* getProcessorInstance(uuid id) {
        return processors[id].get();
    }

};

};
