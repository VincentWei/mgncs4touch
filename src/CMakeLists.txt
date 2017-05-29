MG_ADD_SUB_SOURCE_DIRS (pieces)

MG_ADD_SOURCE_FILES (
    mpad.c mpcomm.c lf_pad.c mpadrdr.c pad_pieces.c
    pad_widget_rdr.c manimation.c mswitchbutton.c mpicker.c
    mnewtrackbar.c mcombopicker.c mtimepicker.c mdatepicker.c
    mexlist.c mbtnnavbar.c mimgnavbar.c mitembar.c
    balloon_tip_maker.c
    )

################ make & install library #####################
IF (NOT disable_shared)
    # shared
    ADD_LIBRARY (${MGNCS4PAD_LIBNAME} SHARED ${_source_list_})
ELSE ()
    # static
    ADD_LIBRARY (${MGNCS4PAD_LIBNAME} STATIC ${_source_list_})
ENDIF (NOT disable_shared)

SET_TARGET_PROPERTIES (${MGNCS4PAD_LIBNAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (${MGNCS4PAD_LIBNAME} PROPERTIES VERSION ${MGNCS4PAD_VERSION} SOVERSION ${MGNCS4PAD_SOVERSION})

TARGET_LINK_LIBRARIES (${MGNCS4PAD_LIBNAME} ${PC_LIBS_LIST})

INSTALL (TARGETS ${MGNCS4PAD_LIBNAME}
    LIBRARY DESTINATION lib     # for .so  (dynamic library)
    ARCHIVE DESTINATION lib     # for .lib (import library)
    RUNTIME DESTINATION bin     # for .dll (dynamic library)
    )
