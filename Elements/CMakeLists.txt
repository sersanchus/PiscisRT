file(GLOB SOURCE_FILES
    "source/*.h"
    "source/*.cpp"
)


add_executable(elements ${SOURCE_FILES})
target_link_libraries(elements PiscisRT)
target_link_libraries(elements PiscisRTut)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(elements ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
