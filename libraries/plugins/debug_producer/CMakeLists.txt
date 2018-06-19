file(GLOB HEADERS "include/hotc/debug_producer/*.hpp")

add_library( hotc_debug_producer
             debug_api.cpp
             debug_producer.cpp
           )

target_link_libraries( hotc_debug_producer hotc_chain hotc_app )
target_include_directories( hotc_debug_producer
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   hotc_debug_producer

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
