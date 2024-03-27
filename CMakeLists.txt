################################################################################
#                                                                              #
#  Copyright (c) 2023, C3D Labs LLC                                            #
#  All rights reserved.                                                        #
#                                                                              #
#  This file is part of C3DVision Library                                      #
#  http://c3dlabs.com                                                          #
#                                                                              #
#  Exclusive rights to C3D Vision, any copies or components (parts) thereof    #
#  and other intellectual property belong to C3D Labs, LLC (the Rightholder).  # 
#  The license rights and restrictions are regulated by the Agreement between  #
#  the User and the Rightholder.                                               #
#                                                                              #
################################################################################

PROJECT(C3DConfigureApplication)
cmake_minimum_required(VERSION 3.14)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DEBUG -D_DRAWGI /Zi /W3 /MP /bigobj") 
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_SECURE_SCL=0 /W3") 
ELSE()
    SET(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DRAWGI -Wno-deprecated-declarations -fno-operator-names -fopenmp")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_SECURE_SCL=0 -Wno-deprecated-declarations -fno-operator-names -fopenmp")
ENDIF()

# Unicode build
OPTION(C3DVision_USING_UNICODE "Enable Unicode support" ON)

OPTION(C3DVision_GUI_QT5_EXAMPLES "Build Qt5 support" ON)
IF (NOT UNIX)
    OPTION(C3DVision_GUI_MFC_EXAMPLES "Build MFC support" ON)
ENDIF()

SET(Examples_OUTPUT "Examples")


# Libraries    
if( NOT C3D_MATH_DIR )
    SET(C3D_MATH_DIR ${CMAKE_SOURCE_DIR}/C3D/Kernel)
endif()

if( NOT C3D_Vision_DIR )
    SET(C3D_Vision_DIR ${CMAKE_SOURCE_DIR}/C3D/Vision)
endif()

# Libraries    
if( NOT C3D_Math_INCLUDE_DIR )
    SET(C3D_Math_INCLUDE_DIR   ${C3D_MATH_DIR}/Include)
endif()

if( NOT C3D_Vision_INCLUDE_DIR )
    SET(C3D_Vision_INCLUDE_DIR ${C3D_Vision_DIR}/Include)
endif()

SET(C3DVSDIR "${Math_SOURCE_DIR}")

SET(C3D_MATH_NAME c3d)
SET(C3D_VIS_LIB c3dvision)

SET(CONFIG_ "$(Configuration)")

if(UNIX)
    SET(CONFIG_ "${CMAKE_BUILD_TYPE}")
endif()

# optimized "${C3D_MATH_DIR}/Release/"
link_directories( "${C3D_MATH_DIR}/${CONFIG_}/" )
# optimized "${C3D_Vision_DIR}/Release/"
link_directories( "${C3D_Vision_DIR}/${CONFIG_}/" )

#binary
set(EXAMPLES_BIN ${CMAKE_BINARY_DIR}/bin/)
if( CMAKE_RUNTIME_OUTPUT_DIRECTORY )
    set(EXAMPLES_BIN ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_BIN})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXAMPLES_BIN})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_BIN})

SET(SHARED_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/Shared)
SET(SRC_Shared 
    ${SHARED_DIR}/scenegenerator.cpp 
    ${SHARED_DIR}/scenegenerator.h
)

IF(C3DVision_USING_UNICODE)
  ADD_DEFINITIONS(
    -DUNICODE
    -D_UNICODE
    )
ENDIF(C3DVision_USING_UNICODE)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Shared
    ${QT_USE_FILE}
    )

IF(UNIX)
    ADD_DEFINITIONS( -std=c++11 )
    SET(STDLIB stdc++)
ELSE()
    SET(STDLIB "")
ENDIF()

message("Math - ${C3D_MATH_DIR}")
If(C3DVision_GUI_QT5_EXAMPLES OR C3DVision_GUI_MFC_EXAMPLES)
    #copy external dll, lib, pdb Math and Vision
    file(COPY "${C3D_MATH_DIR}/Debug/"   DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
    file(COPY "${C3D_MATH_DIR}/Release/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
    file(COPY "${C3D_MATH_DIR}/Release/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo")
    if( NOT "${C3D_Vision_DIR}" STREQUAL "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
        file(COPY "${C3D_Vision_DIR}/Debug/"   DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
        file(COPY "${C3D_Vision_DIR}/Release/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
        file(COPY "${C3D_Vision_DIR}/Release/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo")
    endif()
endif()

###############################################################################
#  QT Examples
###############################################################################
IF (C3DVision_GUI_QT5_EXAMPLES)
    SET(DESIRED_QT_VERSION "5" CACHE STRING "")
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5OpenGL REQUIRED)
  
    ADD_DEFINITIONS(-D__USE_QT__)
    ADD_DEFINITIONS(-DQT_NO_DEPRECATED_WARNINGS)

    SET(QtVision_OUTPUT "qtvision")
    SET(QtVision_DIR QtVision)
    ADD_SUBDIRECTORY(QtVision)

    SET(Examples_Qt_OUTPUT ${Examples_OUTPUT}/Qt)
    INCLUDE_DIRECTORIES(${C3D_Math_INCLUDE_DIR}
        ${C3D_Vision_INCLUDE_DIR}
        ${QtVision_SOURCE_DIR}/Include)
    ADD_DEFINITIONS(-DQT_NO_DEPRECATED_WARNINGS)
    IF((NOT C3DVision_SANDBOX) OR (C3DVision_SANDBOX EQUAL 2))
        ADD_SUBDIRECTORY(Qt)
    ENDIF()
	IF(C3DVision_SANDBOX)
		ADD_SUBDIRECTORY(Sandbox)
	ENDIF()
    ADD_DEFINITIONS( -DNOMINMAX )
ENDIF()
