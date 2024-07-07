cmake_minimum_required(VERSION 3.5)

project(Label2D VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fstack-protector-all")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


include_directories(./)
include_directories(./include)
include_directories(./include/opencv2)
link_directories(./lib)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        CMyDef.h CMyDef.cpp
        CWidget2D.h CWidget2D.cpp
        CImgProcRegister.h
        CImgProc.h CImgProc.cpp
        CImgProcBase.h CImgProcBase.cpp
)

qt_add_executable(Label2D
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    Resource.qrc
)

target_link_libraries(Label2D PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    opencv_world4100.lib
    libopencv_core4100.dll.a
    libopencv_highgui4100.dll.a
    libopencv_imgcodecs4100.dll.a
    libopencv_imgproc4100.dll.a
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Label2D)
endif()
set_target_properties(Label2D PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Label2D
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Label2D)
endif()
