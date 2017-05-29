CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (mGNCS4Pad)

# define if this is a CMAKE PROJECT
ADD_DEFINITIONS (-D__CMAKE_PROJECT__)

SET (MGNCS4PAD_MAJOR_VERSION 1)
SET (MGNCS4PAD_MINOR_VERSION 0)
SET (MGNCS4PAD_MICRO_VERSION 0)

SET (MGNCS4PAD_NAME "mgncs4pad")
SET (MGNCS4PAD_VERSION ${MGNCS4PAD_MAJOR_VERSION}.${MGNCS4PAD_MINOR_VERSION}.${MGNCS4PAD_MICRO_VERSION})
SET (MGNCS4PAD_SOVERSION ${MGNCS4PAD_MAJOR_VERSION})

SET (CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/minigui/cmake/Modules" "${CMAKE_MODULE_PATH}")
INCLUDE (common)

########################## options #########################
MG_DECLARE_OPTION(with_libsuffix      STRING  ""  "configure the suffix of the library name.")
IF (NOT with_libsuffix STREQUAL "")
    SET (LIB_SUFFIX "${with_libsuffix}")
    SET (MGNCS4PAD_LIBNAME ${MGNCS4PAD_NAME}_${LIB_SUFFIX})
ELSE ()
    SET (MGNCS4PAD_LIBNAME ${MGNCS4PAD_NAME})
ENDIF (NOT with_libsuffix STREQUAL "")

OPTION(disable_shared "Disable shared library" OFF)

##################### find dep libs #######################
INCLUDE (FindPkgConfig)

PKG_CHECK_MODULES (MINIGUI minigui)
IF (NOT MINIGUI_FOUND)
    MESSAGE(FATAL_ERROR 
       "MiniGUI is not properly installed on the system. You need 
        MiniGUI Ver 3.0.x or later for building this package. 
        Please configure and install MiniGUI Ver 3.0.x first."
        )
ENDIF (NOT MINIGUI_FOUND)
INCLUDE_DIRECTORIES (${MINIGUI_INCLUDE_DIRS})
LINK_DIRECTORIES (${MINIGUI_LIBRARY_DIRS})
LIST (APPEND PC_LIBS_LIST ${MINIGUI_LIBRARIES})

PKG_CHECK_MODULES (MGNCS mgncs)
IF (NOT MGNCS_FOUND)
    MESSAGE(FATAL_ERROR "mGNCS not found.")
ENDIF (NOT MGNCS_FOUND)
INCLUDE_DIRECTORIES (${MGNCS_INCLUDE_DIRS})
LINK_DIRECTORIES (${MGNCS_LIBRARY_DIRS})
LIST (APPEND PC_LIBS_LIST ${MGNCS_LIBRARIES})

PKG_CHECK_MODULES (MGEFF mgeff)
IF (NOT MGEFF_FOUND)
    MESSAGE(FATAL_ERROR "mGEff not found.")
ENDIF (NOT MGEFF_FOUND)
INCLUDE_DIRECTORIES (${MGEFF_INCLUDE_DIRS})
LINK_DIRECTORIES (${MGEFF_LIBRARY_DIRS})
LIST (APPEND PC_LIBS_LIST ${MGEFF_LIBRARIES})

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/cmake_mgncs4padconfig.h.in"
    "${PROJECT_BINARY_DIR}/mgncs4padconfig.h"
    )
INCLUDE_DIRECTORIES (${PROJECT_BINARY_DIR})

###################### add sub directories #########################
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include")

ADD_SUBDIRECTORY (src)

##################### files install #########################
INSTALL (DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION include/${MGNCS4PAD_NAME}
    FILES_MATCHING PATTERN "*.h"
                   PATTERN ".svn" EXCLUDE
    )
INSTALL (FILES  "${PROJECT_BINARY_DIR}/mgncs4padconfig.h"
    DESTINATION include/${MGNCS4PAD_NAME}
)

INSTALL (FILES  "${PROJECT_SOURCE_DIR}/etc/mgncs4pad.cfg"
    DESTINATION etc
)

# .pc
LIST (APPEND PC_LIBS_LIST ${MGNCS4PAD_LIBNAME})
SET (prefix ${CMAKE_INSTALL_PREFIX})
SET (exec_prefix "\${prefix}")
SET (libdir "\${exec_prefix}/lib")
SET (includedir "\${prefix}/include")
FOREACH (i ${PC_LIBS_LIST})
    SET (PC_LIBS "${PC_LIBS} -l${i}")
ENDFOREACH (i ${PC_LIBS_LIST})
SET (PC_FILE "${PROJECT_BINARY_DIR}/${MGNCS4PAD_NAME}.pc")
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/mgncs4pad.pc.in" ${PC_FILE})
INSTALL (FILES ${PC_FILE} DESTINATION lib/pkgconfig)
