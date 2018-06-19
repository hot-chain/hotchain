file(GLOB HEADERS "include/hotc/app/*.hpp")
file(GLOB EGENESIS_HEADERS "../egenesis/include/hotc/app/*.hpp")

add_library( hotc_app 
             api.cpp
             application.cpp
             plugin.cpp
             ${HEADERS}
             ${EGENESIS_HEADERS}
           )

# need to link hotc_debug_producer because plugins aren't sufficiently isolated #246
target_link_libraries( hotc_app hotc_chain fc hotc_net hotc_utilities hotc_debug_producer )
target_include_directories( hotc_app
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
                            "${CMAKE_CURRENT_SOURCE_DIR}/../egenesis/include" )

if(MSVC)
  set_source_files_properties( application.cpp api.cpp database_api.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

INSTALL( TARGETS
   hotc_app

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/hotc/app" )
