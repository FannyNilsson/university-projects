#--------------------------------------------------------------------------
# GrafikAlgoritmer project
#--------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.8)
PROJECT(GrafikAlgoritmer)
FILE(GLOB GrafikAlgoritmer_headers code/*.h)
FILE(GLOB GrafikAlgoritmer_sources code/*.cc)

SET(files_GrafikAlgoritmer ${GrafikAlgoritmer_headers} ${GrafikAlgoritmer_sources} code/Skeleton.cpp code/Skeleton.h code/Animation.cpp code/Animation.h code/Skinning.cpp code/Skinning.h code/VertexComponents.h code/ShaderObject.cpp code/ShaderObject.h)
SOURCE_GROUP("GrafikAlgoritmer" FILES ${files_GrafikAlgoritmer})

ADD_EXECUTABLE(GrafikAlgoritmer ${files_GrafikAlgoritmer})
TARGET_LINK_LIBRARIES(GrafikAlgoritmer tinyxml glut core render)
ADD_DEPENDENCIES(GrafikAlgoritmer core render)
