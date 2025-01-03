#########
# Setup #
#########

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.5)

# Message
message(STATUS "Pac")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project's name
project(pac)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)

set(PATH_RSYN ${CMAKE_CURRENT_SOURCE_DIR}/rsyn)
set(PATH_PAC ${CMAKE_CURRENT_SOURCE_DIR})

###################
# Warnings/Errors #
###################

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type")

######################
# Check Dependencies #
######################
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
# find_package(PNG REQUIRED)

# OpenMP (work around)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")

#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

###############
# Source Code #
###############
file(GLOB_RECURSE SRC_FILES_RSYN 
    ${PATH_RSYN}/src/*.cpp 
    ${PATH_RSYN}/src/*.cc 
    ${PATH_RSYN}/src/*.c)

file(GLOB_RECURSE SRC_FILES_PAC
    ${PATH_PAC}/src/*.cpp 
    ${PATH_PAC}/src/*.cc 
    ${PATH_PAC}/src/*.c)

set(SRC_FILES ${SRC_FILES_RSYN} ${SRC_FILES_PAC})


#################
# Library Paths #
#################
# Need to come before target is created.

if(LINUX)
link_directories(${PATH_RSYN}/lib/linux)
endif()

if(MACOSX)
link_directories(${PATH_RSYN}/lib/macosx)
endif()

###########
# Targets #
###########

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

#######################
# Include Directories #
#######################

target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_RSYN}/src/rsyn/export")

target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_RSYN}/src")
target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_RSYN}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PATH_PAC}/include")

##################
# Compiler Flags #
##################

target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-unused-result -Wno-format -Wno-return-type -Wno-unused-local-typedefs")

#################
# Linker Flags  #
#################

target_link_libraries(${PROJECT_NAME} lef)
target_link_libraries(${PROJECT_NAME} def)


# Boost   
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    
# PNG
# target_include_directories(${PROJECT_NAME} PUBLIC ${PNG_INCLUDE_DIR})
# target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})

add_subdirectory(python)
