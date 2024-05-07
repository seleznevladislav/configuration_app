PROJECT(application)
cmake_minimum_required(VERSION 3.14)

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
	
	# Add all header files of SOLVER
    file(GLOB SRC_Solver_h CONFIGURE_DEPENDS "./solver/*.h")
    SOURCE_GROUP(Headers FILES ${SRC_Solver_h})

    # Add all source files of SOLVER
    file(GLOB SRC_Solver_cpp CONFIGURE_DEPENDS "./solver/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_Solver_cpp})
		
	########################################################################	
	# Details
	########################################################################	
	# Add all TTOR details source files
    file(GLOB SRC_detailsZarubin_cpp CONFIGURE_DEPENDS "./TTORdetails/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_detailsZarubin_cpp})
	# Add all TTOR GHOSTS
	file(GLOB SRC_detailsTTORGHOST_cpp CONFIGURE_DEPENDS "./TTORdetails/TTOR_GHOST/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_detailsTTORGHOST_cpp})
	    
	# Add all details source files
    file(GLOB SRC_Details_cpp CONFIGURE_DEPENDS "./TTRMdetails/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_TTRMdetails_cpp})

	# Add all details source files
    file(GLOB SRC_IP_details_cpp CONFIGURE_DEPENDS "./IP_details/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_IP_details_cpp})

	# Add all details source files
    file(GLOB SRC_IU_details_cpp CONFIGURE_DEPENDS "./IU_details/*.cpp")
    SOURCE_GROUP(Sources FILES ${SRC_IU_details_cpp})

	
	ADD_EXECUTABLE(
	${ConfigureApplication} WIN32 
	${MOC_Gui} ${RCC_Gui} 
	${SRC_Configurator_h} ${SRC_Configurator_cpp} 
	${SRC_Details_cpp} ${SRC_Shared} 
	${SRC_detailsZarubin_cpp}
	${SRC_detailsTTORGHOST_cpp}
	${SRC_TTRMdetails_cpp}
	${SRC_Solver_h} ${SRC_Solver_cpp}
	${SRC_IP_details_cpp}
	${SRC_IU_details_cpp}
	)
	
    TARGET_LINK_LIBRARIES(${ConfigureApplication} ${C3D_MATH_LIB} ${C3D_VIS_LIB} ${QtVision_OUTPUT} ${OPENGL_LIBRAREIS} ${STDLIB})

    SET_TARGET_PROPERTIES(${ConfigureApplication} PROPERTIES FOLDER ${Examples_OUTPUT}/Qt)
    qt5_use_modules(${ConfigureApplication} Core Gui OpenGL Widgets)
ENDIF()
