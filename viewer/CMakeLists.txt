include_directories(${snu_SOURCE_DIR})
include_directories(${snu_SOURCE_DIR}/lib/enki)
include_directories(${snu_SOURCE_DIR}/lib/enki/viewer)
include_directories(gl)


set(QT_USE_QTOPENGL TRUE)
find_package(Qt4)

if (QT4_FOUND)
	find_package(OpenGL REQUIRED)
	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS})
	
	if (CMAKE_HOST_UNIX)
		add_definitions("-fPIC")
	endif (CMAKE_HOST_UNIX)
	
	set(viewer_lib_SRCS
		Viewer.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/EPuckModel.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/EPuckBody.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/EPuckRest.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/EPuckRing.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/EPuckWheelLeft.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/EPuckWheelRight.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/MarxbotModel.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/MarxbotBase.cpp
		${snu_SOURCE_DIR}/lib/enki/viewer/objects/MarxbotWheel.cpp)
	set(viewer_test_SRCS Playground.cpp)

	qt4_automoc(${viewer_lib_SRCS})
	qt4_automoc(${viewer_test_SRCS})

	qt4_add_resources(viewer_lib_RCC_SRCS ${snu_SOURCE_DIR}/lib/enki/viewer/enki-viewer-textures.qrc)
	
	include_directories (${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/viewer)
	
	link_directories (${enki_BINARY_DIR})
	
	add_library (viewer ${viewer_lib_SRCS} ${viewer_lib_RCC_SRCS})
	add_executable(playground ${viewer_test_SRCS})
	target_link_libraries(playground 
		viewer 
		enki 
		robots 
		worm
		brain
		${QT_LIBRARIES} 
		${OPENGL_LIBRARIES} 
		${EXTRA_LIBS} 
	)
endif (QT4_FOUND)

add_executable(CElegansBehavior CElegansBehavior.cpp)
target_link_libraries(CElegansBehavior robots enki worm brain ${snu_SOURCE_DIR}/viewer/lib/libsxmlgui.a ${snu_SOURCE_DIR}/viewer/lib/libglpng.a glut GLU GL)
