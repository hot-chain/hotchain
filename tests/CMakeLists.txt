file(GLOB COMMON_SOURCES "common/*.cpp")

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling tests with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

file(GLOB UNIT_TESTS "tests/*.cpp")
add_executable( chain_test ${UNIT_TESTS} ${COMMON_SOURCES} )
target_link_libraries( chain_test hotc_native_contract hotc_chain chainbase hotc_utilities hotc_egenesis_none fc ${PLATFORM_SPECIFIC_LIBS} )

if(WASM_TOOLCHAIN)
  file(GLOB SLOW_TESTS "slow_tests/*.cpp")
  add_executable( slow_test ${SLOW_TESTS} ${COMMON_SOURCES} )
  target_link_libraries( slow_test hotc_native_contract hotc_chain chainbase hotc_utilities hotc_egenesis_none fc ${PLATFORM_SPECIFIC_LIBS} )
  target_include_directories( slow_test PUBLIC ${CMAKE_SOURCE_DIR}/contracts )
  add_dependencies(slow_test currency)
endif()