cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(turing-machine-simulator)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

include(FetchContent)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.11.1
)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
  FetchContent_Populate(catch2)
  add_library(catch2 INTERFACE)
  target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND SOURCE_FILES    src/turing_machine_simulator_app.cc
                            src/state.cc
                            src/direction.cc
        src/turing_machine_simulator_helper.cc)

list(APPEND TEST_FILES tests/test_state.cc
                       tests/test_direction.cc
                       tests/test_turing_machine_simulator_helper_methods.cc)

ci_make_app(
  APP_NAME        turing-machine-simulator
  CINDER_PATH     ${CINDER_PATH}
  SOURCES         apps/cinder_app_main.cc ${SOURCE_FILES}
  INCLUDES        include
)

ci_make_app(
  APP_NAME        turing-machine-simulator-test
  CINDER_PATH     ${CINDER_PATH}
  SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
  INCLUDES        include
  LIBRARIES       catch2
)

if(MSVC)
    set_property(TARGET gas-simulation-test APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:CONSOLE")
endif()
