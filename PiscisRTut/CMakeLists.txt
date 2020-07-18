file(GLOB SOURCE_FILES
    "source/*.h"
    "source/*.cpp"
    "extlibs/expatpp/src_pp/*.cpp"
)

ADD_LIBRARY( PiscisRTut STATIC ${SOURCE_FILES} )

add_subdirectory(extlibs/expatpp/expat)
target_link_libraries(PiscisRTut expat)
