add_executable(test_opengl test_opengl.cpp)
target_link_libraries(test_opengl
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  )

add_executable(tri c2_triangle_uniform.cpp)
target_link_libraries(tri
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  sfml-window
  sfml-system
  ${GLEW_LIBRARIES}
  )

add_executable(c6 c6_base.cpp)
target_link_libraries(c6
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${SOIL_LIBRARIES}
  sfml-window
  sfml-system
  ${GLEW_LIBRARIES}
  )
add_executable(z_buffer z_buffer.cpp)
target_link_libraries(z_buffer
  mylib
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${GLUT_LIBRARIES}
  )
