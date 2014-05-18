cmake_minimum_required(VERSION 2.8)

project(ClipGuiGlut)
add_executable(${PROJECT_NAME} 
				renderer.cpp
				clip.h
					)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )