# Create a library called "libFairRutherford" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/example/rutherford/src
${CMAKE_SOURCE_DIR}/example/mcstack
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
FairRutherford.cxx
FairRutherfordContFact.cxx
FairRutherfordGeo.cxx
FairRutherfordGeoPar.cxx
FairRutherfordPoint.cxx
)


Set(HEADERS )
Set(LINKDEF FairRutherfordLinkDef.h)
Set(LIBRARY_NAME FairRutherford)
Set(DEPENDENCIES Base MCStack GeoBase ParBase VMC Physics Cint Core)

GENERATE_LIBRARY()

