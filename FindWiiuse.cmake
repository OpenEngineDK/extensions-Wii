# - Try to find wiiuse
# Once done this will define
#
#  WIIUSE_FOUND - system has ogc
#  WIIUSE_INCLUDE_DIR - the ogc include directory
#  WIIUSE_LIBRARIES - Link these to use ogc
#

FIND_PATH(WIIUSE_INCLUDE_DIR NAMES wiiuse.h
  PATHS
  ${PROJECT_SOURCE_DIR}/libraries/libogc/include
  /libogc/include
  ENV CPATH
  /usr/include
  /usr/local/include
  /opt/local/include
  NO_DEFAULT_PATH
)

FIND_LIBRARY(LIBWIIUSE
  NAMES 
  wiiuse
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

FIND_LIBRARY(LIBBTE
  NAMES 
  bte
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

SET (WIIUSE_LIBRARIES
  ${LIBWIIUSE} 
  ${LIBBTE} 
)

IF(WIIUSE_INCLUDE_DIR AND WIIUSE_LIBRARIES)
   SET(WIIUSE_FOUND TRUE)
   MARK_AS_ADVANCED(WIIUSE_INCLUDE_DIR WIIUSE_LIBRARIES)
ENDIF(WIIUSE_INCLUDE_DIR AND WIIUSE_LIBRARIES)



