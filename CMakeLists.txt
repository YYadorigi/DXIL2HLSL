cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0175 OLD)
project(DXIL2HLSL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Force Release build for main project" FORCE)

# Use ExternalProject to avoid third-party conflicts
include(ExternalProject)

# Step 1: Update submodules
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE result
)
if(NOT result EQUAL "0")
    message(FATAL_ERROR "Failed to update submodules")
endif()

# Step 2: Use ExternalProject to build submodules
# dxil-spirv subproject
ExternalProject_Add(dxil_spirv_external
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor/dxil-spirv
    BINARY_DIR ${CMAKE_BINARY_DIR}/vendor/dxil-spirv
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -B ${CMAKE_BINARY_DIR}/vendor/dxil-spirv -S ${CMAKE_SOURCE_DIR}/vendor/dxil-spirv
    BUILD_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/vendor/dxil-spirv --config Release
    INSTALL_COMMAND ""
)

# SPIRV-Cross subproject
ExternalProject_Add(spirv_cross_external
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor/SPIRV-Cross
    BINARY_DIR ${CMAKE_BINARY_DIR}/vendor/SPIRV-Cross
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -B ${CMAKE_BINARY_DIR}/vendor/SPIRV-Cross -S ${CMAKE_SOURCE_DIR}/vendor/SPIRV-Cross
    BUILD_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/vendor/SPIRV-Cross --config Release
    INSTALL_COMMAND ""
)

# Step 3: Add main project
add_executable(DXIL2HLSL main.cpp)

# Step 4: Set submodule paths
set(DXIL_SPIRV_EXE_PATH ${CMAKE_BINARY_DIR}/vendor/dxil-spirv/Release/dxil-spirv.exe)
set(DXIL_SPIRV_C_SHARED_DLL_PATH ${CMAKE_BINARY_DIR}/vendor/dxil-spirv/Release/dxil-spirv-c-shared.dll)
set(SPIRV_CROSS_EXE_PATH ${CMAKE_BINARY_DIR}/vendor/SPIRV-Cross/Release/spirv-cross.exe)

# Step 5: Copy executable files to main output directory
add_custom_command(TARGET DXIL2HLSL POST_BUILD
    COMMENT "Copying submodule executables to main output directory"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DXIL_SPIRV_EXE_PATH}
        $<TARGET_FILE_DIR:DXIL2HLSL>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DXIL_SPIRV_C_SHARED_DLL_PATH}
        $<TARGET_FILE_DIR:DXIL2HLSL>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SPIRV_CROSS_EXE_PATH}
        $<TARGET_FILE_DIR:DXIL2HLSL>
    DEPENDS dxil_spirv_external spirv_cross_external
)

# Step 6: Generate batch script for RenderDoc plugin
set(BATCH_SCRIPT_CONTENT
"@echo off

: Decompile DXIL to HLSL
\"%~dp0DXIL2HLSL.exe\" \"%1\"

: Redirect to stdout
for %%f in (\"%1\") do type \"%%~dpnf.hlsl\""
)

file(WRITE ${CMAKE_BINARY_DIR}/DXIL2HLSL.bat "${BATCH_SCRIPT_CONTENT}")

add_custom_command(TARGET DXIL2HLSL POST_BUILD
    COMMENT "Generating batch script for RenderDoc plugin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/DXIL2HLSL.bat
        $<TARGET_FILE_DIR:DXIL2HLSL>
    COMMAND ${CMAKE_COMMAND} -E remove
        ${CMAKE_BINARY_DIR}/DXIL2HLSL.bat
    DEPENDS DXIL2HLSL
)