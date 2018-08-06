file(GLOB HEADERS "include/hotc/types/*.hpp")

add_library( hotc_types
             Asset.cpp
             PublicKey.cpp
             TypeParser.cpp
             native.cpp
             ${HEADERS}
             "${CMAKE_CURRENT_SOURCE_DIR}/include/hotc/types/generated.hpp"
             types.hotc
             )
target_include_directories( hotc_types PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_link_libraries( hotc_types hotc_utilities fc )

add_executable( type_generator type_generator.cpp TypeParser.cpp native.cpp )
target_include_directories( type_generator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_link_libraries( type_generator hotc_utilities fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}  )

add_custom_command( OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/include/hotc/types/generated.hpp"
                    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/type_generator ${CMAKE_CURRENT_SOURCE_DIR}/types.hotc ${CMAKE_CURRENT_SOURCE_DIR}/include/hotc/types/generated.hpp
                    DEPENDS types.hotc type_generator )

