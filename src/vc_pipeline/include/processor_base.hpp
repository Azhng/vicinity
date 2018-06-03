#pragma once

#include <vector>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include "./processor_type.hpp"

namespace vc {

using std::vector;
using std::string;
using boost::uuids::uuid;
using boost::uuids::random_generator;

class ProcessorContext;

/**
 * The base class for all pipeline processor subclass.
 * Derive class should override the [[ProcessorBase::processor_function]] method,
 * this method take in a [[ProcessorContext]] object.
 */
class ProcessorBase {

    uuid processor_base_uuid;
    const ProcessorType processor_type;
    vector<string> inports;
    vector<string> outports;

public:

    explicit ProcessorBase(ProcessorType processor_type)
        : processor_base_uuid{random_generator()()},
          processor_type{processor_type} {}

    void run(ProcessorContext* ctx) {
        processor_function(ctx);
    }

    virtual ~ProcessorBase() {}

private:

    // Override this
    virtual void processor_function(ProcessorContext* ctx) = 0;

protected:

    /**
     * method here are meat to be called by the derived class in the constructor to initialize inports/outports
     */

    // Phase 1 API 
    void define_inport(string port_name);

    void define_outport(string port_name);


    // Phase 2 API
    
    /**
     *
     * Implement this using SFINAE enable_if
     *
     * template <typename T>
     * void define_inport(string port_name);
     *
     * template <typename T>
     * void define_outport(string port_name);
     *
     */

};

};
