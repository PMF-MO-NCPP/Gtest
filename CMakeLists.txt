cmake_minimum_required(VERSION 3.14)
project(Gtest CXX)
enable_testing()

find_package(fmt REQUIRED)

########## Stari način kao SUBMODUL  #####################
# Compilation of googletest library #####
# set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")
#
#include_directories(
#    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
#    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
#    )
#
#set(GOOGLETEST_SOURCES
#    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
#    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
#    )
#
#foreach(_source ${GOOGLETEST_SOURCES})
#    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
#endforeach()
#
#add_library(googletest ${GOOGLETEST_SOURCES})
#########################################

###### Novi način ####################################33
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG     e2239ee6043f73722e7aa812a459f54a28552929  # release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
##################################

add_subdirectory(test)
add_subdirectory(src)
