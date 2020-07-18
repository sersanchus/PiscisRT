file(GLOB SOURCE_FILES
    "source/*.h"
    "source/*.cpp"
)

ADD_LIBRARY( PiscisRT STATIC ${SOURCE_FILES} )

