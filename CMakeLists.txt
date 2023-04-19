# Header
cmake_minimum_required(VERSION 3.15)
project(p6 VERSION 1.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Dependencies
find_package(Eigen3 REQUIRED)

execute_process(COMMAND wx-config --cxxflags RESULT_VARIABLE WX_RESULT OUTPUT_VARIABLE WX_CXXFLAGS_OUTPUT ERROR_VARIABLE WX_ERROR)
if (NOT(${WX_RESULT} EQUAL 0) OR NOT("${WX_ERROR}" STREQUAL ""))
    message(FATAL_ERROR "wx-config --cxxflags failed")
endif()
string(REGEX REPLACE "[\n\r]" "" WX_CXXFLAGS_OUTPUT "${WX_CXXFLAGS_OUTPUT}")
string(REGEX REPLACE " " ";" WX_CXXFLAGS_OUTPUT "${WX_CXXFLAGS_OUTPUT}")
execute_process(COMMAND wx-config --libs RESULT_VARIABLE WX_RESULT OUTPUT_VARIABLE WX_LIBS_OUTPUT ERROR_VARIABLE WX_ERROR)
if (NOT(${WX_RESULT} EQUAL 0) OR NOT("${WX_ERROR}" STREQUAL ""))
    message(FATAL_ERROR "wx-config --libs failed")
endif()
string(REGEX REPLACE "[\n\r]" "" WX_LIBS_OUTPUT "${WX_LIBS_OUTPUT}")
string(REGEX REPLACE " " ";" WX_LIBS_OUTPUT "${WX_LIBS_OUTPUT}")
add_library(wxwidgets INTERFACE)
target_compile_options(wxwidgets INTERFACE "${WX_CXXFLAGS_OUTPUT}")
target_link_options(wxwidgets INTERFACE "${WX_LIBS_OUTPUT}")

find_package(GTest)

# Library
add_library(${CMAKE_PROJECT_NAME} SHARED
    "source/p6_common.cpp"
    "source/p6_construction.cpp"
    "source/p6_file.cpp"
    "source/p6_linear_material.cpp"
    "source/p6_material.cpp"
    "source/p6_nonlinear_material.cpp"
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC _USE_MATH_DEFINES)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Eigen3::Eigen)

# GUI
add_executable(${CMAKE_PROJECT_NAME}_gui
    "source/p6_app.cpp"
    "source/p6_force_bar.cpp"
    "source/p6_frame.cpp"
    "source/p6_main_panel.cpp"
    "source/p6_material_bar.cpp"
    "source/p6_menubar.cpp"
    "source/p6_mouse.cpp"
    "source/p6_move_bar.cpp"
    "source/p6_node_bar.cpp"
    "source/p6_side_panel.cpp"
    "source/p6_stick_bar.cpp"
    "source/p6_toolbar.cpp"
)
target_link_libraries(${CMAKE_PROJECT_NAME}_gui PUBLIC ${CMAKE_PROJECT_NAME} wxwidgets)
set_target_properties(${CMAKE_PROJECT_NAME}_gui PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# Test
if(TARGET GTest::gtest)
    add_executable(${CMAKE_PROJECT_NAME}_test
        "source/p6_test.cpp"
    )
    target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC ${CMAKE_PROJECT_NAME} GTest::gtest)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_SOURCE_DIR}/config.cmake.in"
    "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")

write_basic_package_version_file("${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(TARGETS ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_gui
    EXPORT ${CMAKE_PROJECT_NAME}_targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(EXPORT ${CMAKE_PROJECT_NAME}_targets
    FILE "${CMAKE_PROJECT_NAME}Targets.cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")

install(FILES
    "header/p6_common.hpp"
    "header/p6_construction.hpp"
    "header/p6_file.hpp"
    "header/p6_linear_material.hpp"
    "header/p6_material.hpp"
    "header/p6_nonlinear_material.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES
    "icons/area.png"
    "icons/delete.png"
    "icons/force.png"
    "icons/icon.png"
    "icons/move.png"
    "icons/node.png"
    "icons/select.png"
    "icons/simulate.png"
    "icons/stick.png"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}")

install(FILES "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")

# Export
export(EXPORT ${CMAKE_PROJECT_NAME}_targets
    FILE "${CMAKE_PROJECT_NAME}Targets.cmake"
    NAMESPACE ${CMAKE_PROJECT_NAME}::)

# Uninstallation
add_custom_target(uninstall WORKING_DIRECTORY "${CMAKE_BUILD_DIR}" COMMAND cmake -P "${CMAKE_SOURCE_DIR}/uninstall.cmake" ${CMAKE_PROJECT_NAME})

# Documentation
find_package(Doxygen)
if (Doxygen_FOUND)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/documentation")
    add_custom_target(doc WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" COMMAND doxygen)
endif()