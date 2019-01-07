
add_executable(z_buffer z_buffer.cpp)
target_link_libraries(z_buffer
  mylib
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}

  )
