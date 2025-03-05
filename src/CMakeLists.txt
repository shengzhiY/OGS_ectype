
# FIXME: For OS X, need flag for Framework or dylib
IF(DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSG_LIBRARY)
ELSE()
    ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF()

SET(LIB_NAME osg)

#
# Enable workaround for OpenGL driver crash with occlusion query
#

#OPTION(OSG_FORCE_QUERY_RESULT_AVAILABLE_BEFORE_RETRIEVAL "Set to ON to build OcclussionQueryNode with a workaround for multi-threaded OpenGL driver occlussion query crash. " OFF)
#IF(OSG_FORCE_QUERY_RESULT_AVAILABLE_BEFORE_RETRIEVAL)
#    ADD_DEFINITIONS(-DFORCE_QUERY_RESULT_AVAILABLE_BEFORE_RETRIEVAL)
#ENDIF()


SET(HEADER_PATH ${EPIPHYLLUM_SOURCE_DIR}/include/${LIB_NAME})
SET(TARGET_H
	${HEADER_PATH}/LogFileHandler.h
    ${HEADER_PATH}/io_utils
    ${HEADER_PATH}/os_utils
    ${HEADER_PATH}/ApplicationUsage
    ${HEADER_PATH}/ArgumentParser
    #${HEADER_PATH}/BoundingBox
    #${HEADER_PATH}/BoundingSphere
    #${HEADER_PATH}/Callback
    ${HEADER_PATH}/DeleteHandler
    #${HEADER_PATH}/Endian
    ${HEADER_PATH}/Export
    ${HEADER_PATH}/fast_back_stack
    #${HEADER_PATH}/Identifier
    ${HEADER_PATH}/Math
   # ${HEADER_PATH}/Matrix
   # ${HEADER_PATH}/Matrixd
   # ${HEADER_PATH}/Matrixf
   # ${HEADER_PATH}/MixinVector
    ${HEADER_PATH}/Notify
    ${HEADER_PATH}/Object
    ${HEADER_PATH}/observer_ptr
    ${HEADER_PATH}/Observer
   # ${HEADER_PATH}/Quat
    ${HEADER_PATH}/OperationThread
    ${HEADER_PATH}/Referenced
    ${HEADER_PATH}/ref_ptr
  #  ${HEADER_PATH}/Stats
    ${HEADER_PATH}/Timer
  #  ${HEADER_PATH}/Types
    ${HEADER_PATH}/UserDataContainer
  #  ${HEADER_PATH}/ValueObject
  #  ${HEADER_PATH}/ValueMap
  # ${HEADER_PATH}/ValueStack
#   ${HEADER_PATH}/Vec2
#   ${HEADER_PATH}/Vec2b
#   ${HEADER_PATH}/Vec2d
#   ${HEADER_PATH}/Vec2f
#   ${HEADER_PATH}/Vec2i
#   ${HEADER_PATH}/Vec2s
#   ${HEADER_PATH}/Vec2ub
#   ${HEADER_PATH}/Vec2ui
#   ${HEADER_PATH}/Vec2us
#   ${HEADER_PATH}/Vec3
#   ${HEADER_PATH}/Vec3b
#   ${HEADER_PATH}/Vec3d
#   ${HEADER_PATH}/Vec3f
#   ${HEADER_PATH}/Vec3i
#   ${HEADER_PATH}/Vec3s
#   ${HEADER_PATH}/Vec3ub
#   ${HEADER_PATH}/Vec3ui
#   ${HEADER_PATH}/Vec3us
#   ${HEADER_PATH}/Vec4
#   ${HEADER_PATH}/Vec4b
#   ${HEADER_PATH}/Vec4d
#   ${HEADER_PATH}/Vec4f
#   ${HEADER_PATH}/Vec4i
#   ${HEADER_PATH}/Vec4s
#   ${HEADER_PATH}/Vec4ub
#   ${HEADER_PATH}/Vec4ui
#   ${HEADER_PATH}/Vec4us
    ${OPENSCENEGRAPH_VERSION_HEADER}
    ${OPENSCENEGRAPH_CONFIG_HEADER}
#    ${OPENSCENEGRAPH_OPENGL_HEADER}
)

#ADD_LIBRARY(${LIB_NAME}
#    ${OPENSCENEGRAPH_USER_DEFINED_DYNAMIC_OR_STATIC}
#    ${LIB_PUBLIC_HEADERS}
SET(TARGET_SRC
	LogFileHandler.cpp
    os_utils.cpp
    ApplicationUsage.cpp
    ArgumentParser.cpp
   # Callback.cpp
    CopyOp.cpp
    DeleteHandler.cpp
    #Identifier.cpp
    Math.cpp
   # Matrixd.cpp
   # Matrixf.cpp
    Notify.cpp
    Object.cpp
    Observer.cpp
   # Quat.cpp
    OperationThread.cpp
    Referenced.cpp
  #  Stats.cpp
    Timer.cpp
    UserDataContainer.cpp
 #   ValueMap.cpp
 #   ValueStack.cpp
    Version.cpp
    ${OPENSCENEGRAPH_VERSIONINFO_RC}
)


SET(TARGET_LIBRARIES OpenThreads)

SET(TARGET_EXTERNAL_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    ${MATH_LIBRARY}
    ${RT_LIBRARY}
    ${DL_LIBRARY}
)

#LINK_INTERNAL(${LIB_NAME}
#    OpenThreads
#)
#LINK_EXTERNAL(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT}    ${MATH_LIBRARY} ${RT_LIBRARY} ${DL_LIBRARY})
#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY} ${RT_LIBRARY} )

#INCLUDE(ModuleInstall OPTIONAL)

SETUP_LIBRARY(${LIB_NAME})
