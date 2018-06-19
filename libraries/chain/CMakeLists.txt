
file(GLOB HEADERS "include/hotc/chain/*.hpp")
file(GLOB PROTOCOL_HEADERS "include/hotc/chain/protocol/*.hpp")

if( HOTC_DISABLE_UNITY_BUILD )
   set( HOTC_DB_FILES
        db_block.cpp
        db_debug.cpp
        db_getter.cpp
        db_init.cpp
        db_maint.cpp
        db_management.cpp
        db_update.cpp
        db_producer_schedule.cpp
      )
   message( STATUS "Hotc database unity build disabled" )
else( HOTC_DISABLE_UNITY_BUILD )
   set( HOTC_DB_FILES
        database.cpp )
   message( STATUS "Hotc database unity build enabled" )
endif( HOTC_DISABLE_UNITY_BUILD )

## SORT .cpp by most likely to change / break compile
add_library( hotc_chain

             # As database takes the longest to compile, start it first
             ${HOTC_DB_FILES}
             fork_database.cpp

             protocol/types.cpp
             protocol/operations.cpp
             protocol/transaction.cpp
             protocol/block.cpp

             genesis_state.cpp
             get_config.cpp

             evaluator.cpp

             block_database.cpp

             ${HEADERS}
             ${PROTOCOL_HEADERS}
           )

target_link_libraries( hotc_chain fc chainbase )
target_include_directories( hotc_chain
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if(MSVC)
  set_source_files_properties( db_init.cpp db_block.cpp database.cpp block_database.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

INSTALL( TARGETS
   hotc_chain

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/hotc/chain" )
INSTALL( FILES ${PROTOCOL_HEADERS} DESTINATION "include/hotc/chain/protocol" )
