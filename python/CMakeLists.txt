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


add_library(${PROJECT_NAME} SHARED bindings.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_PAC}/include")

target_link_libraries(${PROJECT_NAME} pac ${Python3_LIBRARIES})

# Install rules
install(TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

function(install_with_symlink_and_target lib dest_dir)
    execute_process(COMMAND readlink -f ${lib} OUTPUT_VARIABLE real_file OUTPUT_STRIP_TRAILING_WHITESPACE)
    install(FILES ${lib} DESTINATION ${dest_dir})
    install(FILES ${real_file} DESTINATION ${dest_dir})
endfunction()

foreach(lib ${Python3_LIBRARIES})
    install_with_symlink_and_target(${lib} lib)
endforeach()
