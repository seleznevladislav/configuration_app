################################################################################
#                                                                              #
#  Copyright (c) 2018, C3D Labs LLC                                            #
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
PROJECT(QtVision)
cmake_minimum_required(VERSION 3.4)

SET(DESIRED_QT_VERSION "5" CACHE STRING "")
find_package( Qt5 COMPONENTS Core Gui OpenGL )

SET(QtCore_SRC
    ./src/qt_openglwidget.cpp
    ./src/qt_openglwindow.cpp
    ./src/qt_openglcontext.cpp
    ./src/qt_aboutscenewidget.cpp
    ./src/qt_licensewnd.cpp
    ./src/qt_cursors.cpp
    ./src/qt_headsuptoolbar.cpp
)
SOURCE_GROUP(\\Core FILES ${QtCore_SRC})

SET(MOC_Files
    ./Include/qt_openglwidget.h
    ./Include/qt_openglwindow.h
    ./Include/qt_aboutscenewidget.h
    ./Include/qt_licensewnd.h
    ./Include/qt_headsuptoolbar.h
)
QT5_WRAP_CPP(MOC_Gui ${MOC_Files})

SET(RCC_Files_Vision ./src/qt_openglwidget.qrc)
QT5_ADD_RESOURCES(RCC_Gui ${RCC_Files_Vision})

SET(QtVision_INC
    ./Include/qt_openglcontext.h
    ./Include/qt_openglwidget.h
    ./Include/qt_openglwindow.h
    ./Include/qt_visiondef.h
    ./Include/qt_resstream.h
    ./Include/qt_aboutscenewidget.h
    ./Include/qt_licensewnd.h
    ./Include/qt_cursors.h
    ./Include/qt_headsuptoolbar.h
)
SOURCE_GROUP(Include FILES ${QtVision_INC})

IF(MathTest_NAMES)
    STRING(REPLACE "Source" "Revisions" MATH_INSTALL_DIR ${CMAKE_SOURCE_DIR})
ELSE()
    STRING(REPLACE "Source" "Install" MATH_INSTALL_DIR ${CMAKE_SOURCE_DIR})
ENDIF()

INCLUDE_DIRECTORIES(
    ${VLD_INCLUDE_DIRS}
    Include/
    ${C3D_Math_INCLUDE_DIR}
    ${C3D_Vision_INCLUDE_DIR}
)

IF(MSVC)
ADD_DEFINITIONS( 
    -D_WIN32_WINNT=0X0502
)
ENDIF()

ADD_DEFINITIONS(
    -D_BUILDQTVISIONDLL
    -D__TEST_ONLY__
    -DOGLFT_NO_QT
    -DOGLFT_NO_SOLID
    -DVSN_COMPILER_EXPLICIT_OVERRIDES
    -DNOMINMAX
)

IF (UNIX)
  ADD_DEFINITIONS(
    -DVSN_OS_UNIX
    )
#  ADD_DEFINITIONS(-D_GLIBCXX_USE_CXX11_ABI=0)
ELSE()
  ADD_DEFINITIONS(
    -DVSN_OS_WIN32
    -DVSN_OS_WIN
    )
ENDIF()

IF(MSVC)
  IF(C3DVision_USING_UNICODE)
    ADD_DEFINITIONS( 
      -DUNICODE
      -D_UNICODE
#      -DVSN_DEBUG
      )
  ENDIF(C3DVision_USING_UNICODE)
ENDIF()

IF(C3DVision_WITH_VLD)
  ADD_DEFINITIONS(
    -DENABLE_VLD
  )
ENDIF(C3DVision_WITH_VLD)

ADD_DEFINITIONS( 
    -D__USE_QT__
)

IF(C3DVision_WITH_INSTALLER)
    IF(DEFINED ENV{C3DVision_INSTALL_PREFIX})
      ADD_DEFINITIONS(
        -DQT_VISION_STATIC
        -DQT_VISION_EXPORT
      )
    ENDIF()
ENDIF()

IF(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DEBUG -D_DRAWGI /Zi /W3 /MP /bigobj")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_SECURE_SCL=0 /W3")
ELSE()
    SET(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG_INIT} -std=c++11 -D_DEBUG -D_DRAWGI -Wno-deprecated-declarations -fno-operator-names -fopenmp")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -std=c++11 -D_SECURE_SCL=0 -Wno-deprecated-declarations -fno-operator-names -fopenmp")
ENDIF()

# метод сборки библиотеки
SET(QtVisionBuildMethod SHARED)
IF(C3DVision_WITH_INSTALLER)
    IF(DEFINED ENV{C3DVision_INSTALL_PREFIX})
        SET(QtVisionBuildMethod STATIC)
    ENDIF()
ENDIF()
  
ADD_LIBRARY(${QtVision_OUTPUT} ${QtVisionBuildMethod} 
    ${QtCore_SRC} 
    ${QtVision_INC} 
    ${MOC_Gui}
    ${RCC_Gui}
)

message(STATUS "qtvision cxx: ${CMAKE_CXX_FLAGS}")

QT5_USE_MODULES(${QtVision_OUTPUT} Core Gui OpenGL Widgets)

IF (UNIX)
    SET(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++")
ENDIF()

IF(MSVC) 
    TARGET_LINK_LIBRARIES(
        ${QtVision_OUTPUT}
        ${C3D_VIS_LIB}
        ${C3D_MATH_NAME}
        opengl32
        glu32
    )
ELSE()
    find_package(OpenGL REQUIRED)
    TARGET_LINK_LIBRARIES(
        ${QtVision_OUTPUT}
        ${C3D_VIS_LIB}
        ${C3D_MATH_NAME}
        ${OPENGL_LIBRARIES}
        ${GLU_LIBRARY}
    )
ENDIF()

IF(NOT ${C3DVision_INSTALL_DIR} STREQUAL "")
    INSTALL(TARGETS ${QtVision_OUTPUT}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${C3DVision_INSTALL_DIR}/Release
        LIBRARY DESTINATION ${C3DVision_INSTALL_DIR}/Release
        ARCHIVE DESTINATION ${C3DVision_INSTALL_DIR}/Release)
    INSTALL(TARGETS ${QtVision_OUTPUT}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${C3DVision_INSTALL_DIR}/Debug
        LIBRARY DESTINATION ${C3DVision_INSTALL_DIR}/Debug
        ARCHIVE DESTINATION ${C3DVision_INSTALL_DIR}/Debug)
    INSTALL(DIRECTORY ${QtVision_SOURCE_DIR}/Include DESTINATION ${C3DVision_INSTALL_DIR}
            FILES_MATCHING PATTERN "*.h")
    ADD_CUSTOM_COMMAND(TARGET ${QtVision_OUTPUT}
    POST_BUILD
    COMMAND cmake -DBUILD_TYPE=$<CONFIGURATION> -P cmake_install.cmake)
ENDIF()


#IF (C3DVision_FOR_VIEWER AND C3DVision_POCKET_DIRECTORY)
#  include(vision_support)
#  pocket_copy_include("${QtVision_INC}" "${C3DVision_POCKET_DIRECTORY}/../Include")
#  pocket_output(${QtVision_OUTPUT})
#  pocket_install_command(${QtVision_OUTPUT})
#ENDIF()
