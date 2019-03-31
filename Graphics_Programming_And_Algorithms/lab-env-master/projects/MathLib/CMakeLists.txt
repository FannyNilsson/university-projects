#--------------------------------------------------------------------------
# MathLib project
#--------------------------------------------------------------------------

PROJECT(MathLib)
FILE(GLOB MathLib_headers code/*.h)
FILE(GLOB MathLib_sources code/*.cpp)

SET(files_MathLib ${MathLib_headers} ${MathLib_sources})
SOURCE_GROUP("MathLib" FILES ${files_MathLib})
INCLUDE_DIRECTORIES(code)
ADD_EXECUTABLE(MathLib ${files_MathLib})
TARGET_LINK_LIBRARIES(MathLib core render)
#ADD_DEPENDENCIES(MathLib core render)