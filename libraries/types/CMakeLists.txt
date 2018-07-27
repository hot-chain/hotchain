file(GLOB HEADERS "include/HOTC/types/*.hpp")

add_library( HOTC_hotc_types
             Asset.cpp
             PublicKey.cpp
             TypeParser.cpp
             native.cpp
             ${HEADERS}
             "${CMAKE_CURRENT_SOURCE_DIR}/include/HOTC/types/generated.hpp"
             types.HOTC_hotc
             )
target_include_directories( HOTC_hotc_types PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_link_libraries( HOTC_hotc_types HOTC_hotc_utilities fc )

add_executable( type_generator type_generator.cpp TypeParser.cpp native.cpp )
target_include_directories( type_generator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
target_link_libraries( type_generator HOTC_hotc_utilities fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}  )

add_custom_command( OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/include/HOTC/types/generated.hpp"
                    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/type_generator ${CMAKE_CURRENT_SOURCE_DIR}/types.HOTC_hotc ${CMAKE_CURRENT_SOURCE_DIR}/include/HOTC/types/generated.hpp
                    DEPENDS types.HOTC_hotc type_generator )

