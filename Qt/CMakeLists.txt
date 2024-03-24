PROJECT(application)
cmake_minimum_required(VERSION 3.4)

IF(C3DVision_GUI_QT5_EXAMPLES)
    SET(ConfigureApplication "application")

    # Add all header files
    file(GLOB SRC_Configurator_h CONFIGURE_DEPENDS "*.h")
    SOURCE_GROUP(Headers FILES ${SRC_Configurator_h})

    # Add all source files
    file(GLOB SRC_Configurator_cpp CONFIGURE_DEPENDS "*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_Configurator_cpp})

    # Set MOC files
    SET(MOC_Files ${SRC_Configurator_h})
    QT5_WRAP_CPP(MOC_Gui ${MOC_Files})

    # Set Rcc files files
    SET(RCC_Files_Explode ./explodewidget.qrc)
    QT5_ADD_RESOURCES(RCC_Gui ${RCC_Files_Explode})

    # Add all details source files
    file(GLOB SRC_Details_cpp CONFIGURE_DEPENDS "./details/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_Details_cpp})

    ADD_EXECUTABLE(${ConfigureApplication} WIN32 ${MOC_Gui} ${RCC_Gui} ${SRC_Configurator_h} ${SRC_Configurator_cpp} ${SRC_Details_cpp} ${SRC_Shared})
    TARGET_LINK_LIBRARIES(${ConfigureApplication} ${C3D_MATH_LIB} ${C3D_VIS_LIB} ${QtVision_OUTPUT} ${OPENGL_LIBRAREIS} ${STDLIB})

    SET_TARGET_PROPERTIES(${ConfigureApplication} PROPERTIES FOLDER ${Examples_OUTPUT}/Qt)
    qt5_use_modules(${ConfigureApplication} Core Gui OpenGL Widgets)
ENDIF()
