#pragma once

#include <vector>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include "./processor_type.hpp"

namespace vc {

namespace core {

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
    bool ui_thread; // to handle OpenCV UI lib shanigens

public:

    explicit ProcessorBase(ProcessorType processor_type, bool _ui_thread = false)
        : processor_base_uuid{random_generator()()},
          processor_type{processor_type},
          ui_thread{_ui_thread} {}

    virtual void run(ProcessorContext* ctx) = 0;

    const ProcessorType& getProcessorType() const {
        return processor_type;
    }

    const vector<string>& getInportNames() const {
        return inports;
    }

    const vector<string>& getOutportNames() const {
        return outports;
    }

    bool runOnUIThread() const {
        return ui_thread;
    }

    virtual ~ProcessorBase() {}


protected:

    /**
     * method here are meant to be called by the derived class in the constructor to initialize inports/outports
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

};
