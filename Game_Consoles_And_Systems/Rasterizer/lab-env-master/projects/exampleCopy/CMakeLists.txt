#--------------------------------------------------------------------------
# exampleCopy project
#--------------------------------------------------------------------------

PROJECT(exampleCopy)
FILE(GLOB exampleCopy_headers code/*.h)
FILE(GLOB exampleCopy_sources code/*.cc)

SET(files_exampleCopy ${exampleCopy_headers} ${exampleCopy_sources})
SOURCE_GROUP("exampleCopy" FILES ${files_exampleCopy})

ADD_EXECUTABLE(exampleCopy ${files_exampleCopy})
TARGET_LINK_LIBRARIES(exampleCopy core render)
ADD_DEPENDENCIES(exampleCopy core render)