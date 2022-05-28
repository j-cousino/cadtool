cmake_minimum_required(VERSION 3.16.0)

project(cadtool VERSION 0.1.0
    DESCRIPTION "A tool for modeling physical objects"
    HOMEPAGE_URL "https://github.com/j-cousino/cadtool" )

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    set(GITSCM_INUSE TRUE)

    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
        )
    string(STRIP ${PROJECT_VERSION_TWEAK} PROJECT_VERSION_TWEAK)

    set(CMAKE_PROJECT_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})

    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_GIT_COMMIT)
    string(STRIP ${PROJECT_GIT_COMMIT} PROJECT_GIT_COMMIT)

    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
    
    set(GIT_SUBMODULE_LIST  extern/wxWidgets)

    foreach(SUBM ${GIT_SUBMODULE_LIST} )
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${SUBM}/CMakeLists.txt")
            message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
        else()
            add_subdirectory(${PROJECT_SOURCE_DIR}/${SUBM})
        endif()
    endforeach()
endif()

# import the OpenCascade targets
include(c:/OpenCascade/cmake/OpenCascadeConfig.cmake)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/config.h.in
    ${PROJECT_BINARY_DIR}/include/config.h
    )

add_executable(cadtool 
    srcs/main.cpp 
    srcs/oc3dView.cpp 
    srcs/ocWindow.cpp)

target_link_libraries(cadtool wxcore wxbase freeglut opengl32
    ${OpenCASCADE_Visualization_LIBRARIES}
    ${OpenCASCADE_FoundationClasses_LIBRARIES}
    )

target_include_directories(cadtool PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${OpenCASCADE_INCLUDE_DIR})

# Testing
include(CTest)
enable_testing()

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
