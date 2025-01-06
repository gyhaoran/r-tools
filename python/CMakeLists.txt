project(pacpy)

if(NOT DEFINED PKGS_PATH)
    set(PKGS_PATH /home/usergrp/data/pkgs)
endif()

message(STATUS "PKGS_PATH: ${PKGS_PATH}")

set(PYBIND11_PATH ${PKGS_PATH}/pybind11)
list(APPEND CMAKE_PREFIX_PATH ${PYBIND11_PATH})

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# Find Python and pybind11
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(pybind11 REQUIRED)

# Print Python3 details for debugging
message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")
message(STATUS "Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARIES: ${Python3_LIBRARIES}")

pybind11_add_module(${PROJECT_NAME} bindings.cpp)

# add_library(${PROJECT_NAME} SHARED bindings.cpp)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     PREFIX ""
#     SUFFIX ${PYTHON3_MODULE_EXTENSION}
# )

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     PREFIX ""
#     SUFFIX ".so"
# )
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_PAC}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE pac)

set_target_properties(pacpy PROPERTIES
    INSTALL_RPATH "$ORIGIN"
)
