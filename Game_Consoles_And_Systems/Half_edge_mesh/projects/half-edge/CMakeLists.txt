#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(half-edge)
FILE(GLOB half-edge_headers code/*.h)
FILE(GLOB half-edge_sources code/*.cc)

SET(files_half-edge ${half-edge_headers} ${half-edge_sources} code/Half_Edge_Mesh.cpp code/Half_Edge_Mesh.h)
SOURCE_GROUP("half-edge" FILES ${files_half-edge})

ADD_EXECUTABLE(half-edge ${files_half-edge})
TARGET_LINK_LIBRARIES(half-edge core render)
ADD_DEPENDENCIES(half-edge core render)
