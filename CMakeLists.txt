project(Hierarchical-Z-Buffer)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_CXX_FLAGS "-fopenmp -std=c++11 -o3 -fpermissive -DGLEW_STATIC")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/external)


# BOOST
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS filesystem system program_options REQUIRED)
if(Boost_FOUND)
  message("-- Boost @ ${Boost_INCLUDE_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)


#GLM
find_package(GLM REQUIRED)
if(GLM_FOUND)
  message("FIND GLM")
  include_directories(${GLM_INCLUDE_DIR})
endif(EIGEN3_FOUND)

# EIGEN3
find_package(Eigen3 REQUIRED)
if(EIGEN3_FOUND)
  message("-- Eigen @ ${EIGEN3_INCLUDE_DIR}")
  message("-- Eigen version: ${EIGEN3_VERSION}")
  include_directories(${EIGEN3_INCLUDE_DIR})
endif(EIGEN3_FOUND)

#GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()

#GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})


#OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

#SFM
set(sfml-system /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.4)
set(sfml-window /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.4)
# pkg_search_module(GLFW REQUIRED sfml)
# include_directories(${SFML_INCLUDE_DIRS})
# add_subdirectory(src)

#SOIL
set(SOIL_INCLUDE_DIRS /usr/include/SOIL/)
include_directories(${SOIL_INCLUDE_DIRS})
set(SOIL_LIBRARIES /usr/lib/libSOIL.so)

#GLUT
set(GLUT_LIBRARIES /usr/lib/x86_64-linux-gnu/libglut.so)



add_subdirectory(src)

add_subdirectory(tests)
