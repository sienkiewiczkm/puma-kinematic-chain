cmake_minimum_required(VERSION 3.0)
project(puma-chain-simulation)

set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_NAME_LIB ${PROJECT_NAME}-lib)
set(DEPENDENCIES_DIR dependencies/)

add_subdirectory(${DEPENDENCIES_DIR}/framework)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
    ${FRAMEWORK_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME_LIB}
    source/Application.cpp
    source/CommonMath.cpp
    source/PumaCalculator.cpp
    source/PumaConfiguration.cpp
    source/PumaConfigurationWindow.cpp
    source/PumaIKSolver.cpp
    source/PumaModel.cpp
)

add_executable(${PROJECT_NAME}
    source/Main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME_LIB}
    ${FRAMEWORK_LIBRARIES}
)

set(PROJECT_COMPILE_FEATURES
    ${PROJECT_COMPILE_FEATURES}
    cxx_auto_type
    cxx_nullptr
    cxx_range_for
)

target_compile_features(${PROJECT_NAME} PRIVATE
    ${PROJECT_COMPILE_FEATURES}
)

target_compile_features(${PROJECT_NAME_LIB} PRIVATE
    ${PROJECT_COMPILE_FEATURES}
)

