project(pacpy)

if(NOT DEFINED PKGS_PATH)
    set(PKGS_PATH /home/usergrp/data/pkgs)
endif()

set(PYBIND11_PATH ${PKGS_PATH}/pybind11)
list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_PATH})

find_package(pybind11 REQUIRED)
pybind11_add_module(${PROJECT_NAME} bindings.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_PAC}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE pac)

set_target_properties(pacpy PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)
