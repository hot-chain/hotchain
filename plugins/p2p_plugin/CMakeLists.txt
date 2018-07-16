file(GLOB HEADERS "include/hotc/p2p_plugin/*.hpp")

add_library( p2p_plugin
             p2p_plugin.cpp
             ${HEADERS}
           )

target_link_libraries( p2p_plugin chain_plugin appbase hotc_chain hotc_utilities hotc_net )
target_include_directories( p2p_plugin
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   p2p_plugin

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
