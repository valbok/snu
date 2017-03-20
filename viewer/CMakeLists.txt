include_directories(${snu_SOURCE_DIR})
include_directories(${snu_SOURCE_DIR}/lib/enki)
include_directories(${snu_SOURCE_DIR}/lib/enki/viewer)

find_package(SDL)
if (SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
	set(EXTRA_LIBS ${SDL_LIBRARY} ${EXTRA_LIBS})
	add_definitions("-DUSE_SDL")
	message("SDL found")
endif (SDL_FOUND)

set(QT_USE_QTOPENGL TRUE)
find_package(Qt5)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5OpenGL REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (OPENGL_FOUND)
	message ("OPENGL found")
else (OPENGL_FOUND)
	message ("OPENGL not found")
endif (OPENGL_FOUND)

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
	${snu_SOURCE_DIR}/lib/enki/viewer/objects/MarxbotWheel.cpp
	${snu_SOURCE_DIR}/lib/enki/viewer/Thymio2Model.cpp
	${snu_SOURCE_DIR}/lib/enki/viewer/objects/Thymio2Body.cpp
	${snu_SOURCE_DIR}/lib/enki/viewer/objects/Thymio2Wheel.cpp
	)
set(viewer_test_SRCS Playground.cpp)

qt5_add_resources(viewer_lib_RCC_SRCS ${snu_SOURCE_DIR}/lib/enki/viewer/enki-viewer-textures.qrc)

include_directories (${PROJECT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/viewer)

link_directories (${enki_BINARY_DIR})

add_library (viewer ${viewer_lib_SRCS} ${viewer_lib_RCC_SRCS} )
qt5_use_modules(viewer OpenGL)
add_executable(playground ${viewer_test_SRCS})
target_link_libraries(playground viewer enki robots worm brain Qt5::Core Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${EXTRA_LIBS})

#include_directories(gl)
#link_directories (lib)
#add_executable(CElegansBehavior CElegansBehavior.cpp)
#target_link_libraries(CElegansBehavior robots enki worm brain Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${EXTRA_LIBS} libsxmlgui.a libglpng.a glut GLU GL)
