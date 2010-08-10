# - Try to find ogc
# Once done this will define
#
#  OGC_FOUND - system has ogc
#  OGC_INCLUDE_DIR - the ogc include directory
#  OGC_LIBRARIES - Link these to use ogc
#

FIND_PATH(OGC_INCLUDE_DIR NAMES gccore.h
  PATHS
  ${PROJECT_SOURCE_DIR}/libraries/libogc/include
  /libogc/include
  ENV CPATH
  /usr/include
  /usr/local/include
  /opt/local/include
  NO_DEFAULT_PATH
)

FIND_LIBRARY(LIBOGC
  NAMES 
  ogc
  PATHS
  /libogc/lib/wii
  ${PROJECT_SOURCE_DIR}/libraries/libogc/lib/wii
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)

SET (OGC_LIBRARIES
  ${LIBOGC} 
)

IF(OGC_INCLUDE_DIR AND OGC_LIBRARIES)
   SET(OGC_FOUND TRUE)
   MARK_AS_ADVANCED(OGC_INCLUDE_DIR OGC_LIBRARIES)
ENDIF(OGC_INCLUDE_DIR AND OGC_LIBRARIES)



