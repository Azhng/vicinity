file(GLOB vc_core pipeline_core/*.cpp)

add_library(vic_core STATIC ${vc_core})

subdirs(pipeline_extensions)
