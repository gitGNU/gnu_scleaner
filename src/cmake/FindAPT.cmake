# - Find the libapt-pkg includes and library
#

# This module defines
#  APT_PKG_INCLUDE_DIR, where to find pkgcache.h, etc.
#  APT_PKG_LIBRARIES, the libraries to link against to use APT-PKG.
#  APT_GET_DEFINITIONS - You should ADD_DEFINITONS(${PNG_DEFINITIONS}) before compiling code that includes png library files.
#  APT_GET_FOUND, If false, do not try to use PNG.
# also defined, but not for general use are
#  APT_GET_LIBRARY, where to find the PNG library.

SET(APT_GET_FOUND "NO")

FIND_PATH(APT_PKG_INCLUDE_DIR pkgcache.h /usr/include/apt-pkg/)

SET(PNG_NAMES ${PNG_NAMES} png libpng)
  FIND_LIBRARY(PNG_LIBRARY
    NAMES ${PNG_NAMES}
    PATHS /usr/lib /usr/local/lib
  )

  IF (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)
      # png.h includes zlib.h. Sigh.
      SET(PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )
      SET(PNG_LIBRARIES ${PNG_LIBRARY} ${ZLIB_LIBRARY})
      SET(PNG_FOUND "YES")

      IF (CYGWIN)
        IF(BUILD_SHARED_LIBS)
           # No need to define PNG_USE_DLL here, because it's default for Cygwin.
        ELSE(BUILD_SHARED_LIBS)
          SET (PNG_DEFINITIONS -DPNG_STATIC)
        ENDIF(BUILD_SHARED_LIBS)
      ENDIF (CYGWIN)

  ENDIF (PNG_LIBRARY AND PNG_PNG_INCLUDE_DIR)

ENDIF(ZLIB_FOUND)

IF (PNG_FOUND)
  IF (NOT PNG_FIND_QUIETLY)
    MESSAGE(STATUS "Found PNG: ${PNG_LIBRARY}")
  ENDIF (NOT PNG_FIND_QUIETLY)
ELSE (PNG_FOUND)
  IF (PNG_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find PNG library")
  ENDIF (PNG_FIND_REQUIRED)
ENDIF (PNG_FOUND)

MARK_AS_ADVANCED(PNG_PNG_INCLUDE_DIR PNG_LIBRARY )
