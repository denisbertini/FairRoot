# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/example/mcstack
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/example/mcstack

)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
FairStack.cxx 
FairMCTrack.cxx
)

Set(HEADERS )
Set(LINKDEF MCStackLinkDef.h)
Set(LIBRARY_NAME MCStack)
Set(DEPENDENCIES Base EG Physics Cint Core)

GENERATE_LIBRARY()

