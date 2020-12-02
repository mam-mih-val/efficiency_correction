cmake_minimum_required(VERSION 3.15)
project(EfficiencyCorrection)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH $ENV{QnTools_HOME})
list(APPEND CMAKE_PREFIX_PATH ${QnTools_HOME})

find_package(ROOT REQUIRED COMPONENTS RIO Tree)
find_package(QnTools REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
include(${ROOT_USE_FILE})

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${ROOT_INCLUDE_DIRS}
        ${QnTools_INCLUDE_DIR}/QnTools
)

add_executable(correct src/correct.cc)
target_link_libraries(correct QnTools::Base ${ROOT_LIBRARIES})

add_executable(calculate_a src/calculate_a.cc)
target_link_libraries(calculate_a QnTools::Base ${ROOT_LIBRARIES})

add_executable(calculate_e src/calculate_e.cc)
target_link_libraries(calculate_e QnTools::Base ${ROOT_LIBRARIES})