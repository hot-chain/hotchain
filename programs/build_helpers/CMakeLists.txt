
add_executable( cat-parts cat-parts.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

# we only actually need Boost, but link against FC for now so we don't duplicate it.
target_link_libraries( cat-parts PRIVATE fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

add_executable( member_enumerator member_enumerator.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

# we only actually need Boost, but link against FC for now so we don't duplicate it.
target_link_libraries( member_enumerator PRIVATE fc hotc_app hotc_net hotc_chain hotc_egenesis_brief hotc_utilities ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

