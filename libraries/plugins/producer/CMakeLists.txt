file(GLOB HEADERS "include/hotc/producer/*.hpp")

add_library( hotc_producer 
             producer.cpp
           )

target_link_libraries( hotc_producer appbase hotc_chain hotc_utilities )
target_include_directories( hotc_producer
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   hotc_producer

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
