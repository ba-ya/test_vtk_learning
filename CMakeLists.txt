cmake_minimum_required(VERSION 3.16)

project(test_vtk_learning VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(VTK_path "D:/proj/framework/3rdparty/vtk/windows/lib/cmake/vtk-9.4")
list(APPEND CMAKE_PREFIX_PATH "${VTK_path}")
find_package(VTK REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE
#   ${VTK_LIBRARIES}
# )
# if(VTK_VERSION VERSION_GREATER_EQUAL "9.0")
#     target_link_libraries(${PROJECT_NAME} PRIVATE
#         VTK::GUISupportQt
#         VTK::RenderingOpenGL2
#     )
# endif()

include_directories(
  ./
  examples/
)

set(MAINWIDGET
  func1.cpp
  func1.h
  func1.ui
)

set(EXAMPLE
  00headers.h
  examples/01-25/01cells.cpp
  examples/01-25/02helloworld.cpp
  examples/01-25/03simple_operations.cpp
  examples/01-25/04axes.cpp
  examples/01-25/05color_lines.cpp
  examples/01-25/06cube.cpp
  examples/01-25/07dodecahedron.cpp
  examples/01-25/08EllipticalCylinder.cpp
  examples/01-25/09GeometricObjectsDemo.cpp
  examples/01-25/10PlaneSourceDemo.cpp
  examples/01-25/11Planes.cpp
  examples/01-25/12ShrinkCube.cpp
  examples/01-25/13SourceObjectsDemo.cpp
  examples/01-25/14Cell3DDemonstration.cpp
  examples/01-25/15CellTypeSource.cpp
  examples/01-25/16ConvexPointSet.cpp
  examples/01-25/17Assembly.cpp
  examples/01-25/18LinearCellsDemo.cpp
  examples/01-25/19ConesOnSphere.cpp
  examples/01-25/20EarthSource.cpp
  examples/01-25/21Frustum.cpp
  examples/01-25/22OrientedArrow.cpp
  examples/01-25/23OrientedCylinder.cpp
  examples/01-25/24Plane.cpp
  examples/01-25/25PlatonicSolids.cpp
  examples/26-50/26RegularPolygonSource.cpp
  examples/26-50/27TessellatedBoxSource.cpp
  examples/26-50/28IsoparametricCellsDemo.cpp
  examples/26-50/29LegendScaleActor.cpp
  examples/26-50/30PolarAxesActor.cpp
  examples/26-50/31TextOrigin.cpp
  examples/26-50/32XYPlot.cpp
  examples/26-50/33ProbeCombustor.cpp
  examples/26-50/34ColorIsosurface.cpp
)

set(PROJECT_SOURCES
        main.cpp
        common.h
        ${MAINWIDGET}
        ${EXAMPLE}
        icon.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_vtk_learning
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET test_vtk_learning APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(test_vtk_learning SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(test_vtk_learning
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  ${VTK_LIBRARIES}
)

if(VTK_VERSION VERSION_GREATER_EQUAL "9.0")
    target_link_libraries( ${PROJECT_NAME} PRIVATE
        VTK::GUISupportQt
        VTK::RenderingOpenGL2
    )
endif()
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.test_vtk_learning)
endif()
set_target_properties(test_vtk_learning PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS test_vtk_learning
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test_vtk_learning)
endif()
