file(GLOB header *.h)
file(GLOB source *.cc *.cpp)
add_library(mylib SHARED ${source} )
target_link_libraries(mylib
  ${GLFW_LIBRARIES}
  )
