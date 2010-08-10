# - Try to find ogc
# Once done this will define
#
#  FAT_FOUND - system has ogc
#  FAT_INCLUDE_DIR - the ogc include directory
#  FAT_LIBRARIES - Link these to use ogc
#

FIND_PATH(FAT_INCLUDE_DIR NAMES fat.h
  PATHS
  ${PROJECT_SOURCE_DIR}/libraries/libfat-ogc/include
  /libfat-ogc/include
  ENV CPATH
  /usr/include
  /usr/local/include
  /opt/local/include
  NO_DEFAULT_PATH
)

FIND_LIBRARY(LIBFAT
  NAMES 
  fat
  PATHS
  /libfat-ogc/lib/wii
  ${PROJECT_SOURCE_DIR}/libraries/libfat-ogc/lib/wii
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)

SET (FAT_LIBRARIES
  ${LIBFAT} 
)

IF(FAT_INCLUDE_DIR AND FAT_LIBRARIES)
   SET(FAT_FOUND TRUE)
   MARK_AS_ADVANCED(FAT_INCLUDE_DIR FAT_LIBRARIES)
ENDIF(FAT_INCLUDE_DIR AND FAT_LIBRARIES)



