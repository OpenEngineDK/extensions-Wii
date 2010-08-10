# - Try to find remote debug lib
# Once done this will define
#
#  DB_FOUND - system has ogc
#  DB_INCLUDE_DIR - the ogc include directory
#  DB_LIBRARIES - Link these to use ogc
#

FIND_PATH(DB_INCLUDE_DIR NAMES debug.h
  PATHS
  ${PROJECT_SOURCE_DIR}/libraries/libogc/include
  /libogc/include
  ENV CPATH
  /usr/include
  /usr/local/include
  /opt/local/include
  NO_DEFAULT_PATH
)

FIND_LIBRARY(LIBDB
  NAMES 
  db
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

SET (DB_LIBRARIES
  ${LIBDB} 
)

IF(DB_INCLUDE_DIR AND DB_LIBRARIES)
   SET(DB_FOUND TRUE)
   MARK_AS_ADVANCED(DB_INCLUDE_DIR DB_LIBRARIES)
ENDIF(DB_INCLUDE_DIR AND DB_LIBRARIES)



