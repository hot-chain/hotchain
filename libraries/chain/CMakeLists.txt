
file(GLOB HEADERS "include/HOTC/chain/*.hpp")
file(GLOB PROTOCOL_HEADERS "include/HOTC/chain/protocol/*.hpp")

## SORT .cpp by most likely to change / break compile
add_library( HOTC_chain
             database.cpp
             sys_contract.cpp
             fork_database.cpp

             protocol/types.cpp
             protocol/transaction.cpp
             protocol/block.cpp

             genesis_state.cpp
             get_config.cpp

             block_log.cpp

             ${HEADERS}
             ${PROTOCOL_HEADERS}
           )

target_link_libraries( HOTC_chain fc chainbase )
target_include_directories( HOTC_chain
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if(MSVC)
  set_source_files_properties( db_init.cpp db_block.cpp database.cpp block_log.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

INSTALL( TARGETS
   HOTC_chain

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/HOTC/chain" )
INSTALL( FILES ${PROTOCOL_HEADERS} DESTINATION "include/HOTC/chain/protocol" )
