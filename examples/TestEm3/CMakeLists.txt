# SPDX-FileCopyrightText: 2021 CERN
# SPDX-License-Identifier: Apache-2.0

if(NOT TARGET g4HepEm)
  message(STATUS "Disabling TestEm3 (needs G4HepEm)")
  return()
endif()

# TestEm3 for validation of particle transportation with GPUs.
add_executable(TestEm3 TestEm3.cpp TestEm3.cu)
target_include_directories(TestEm3 PUBLIC
  $<TARGET_PROPERTY:g4HepEmRun,INCLUDE_DIRECTORIES>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/base/inc>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/physics/processes/inc>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tracking/inc>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/magneticfield/inc>
  $<INSTALL_INTERFACE:base>
)
target_compile_definitions(TestEm3 PRIVATE G4HepEm_CUDA_BUILD)
target_link_libraries(TestEm3 PRIVATE VecCore::VecCore VecGeom::vecgeom VecGeom::vecgeomcuda_static VecGeom::vgdml ${Geant4_LIBRARIES} g4HepEmData g4HepEmInit CopCore::CopCore)
target_compile_options(TestEm3 PRIVATE "$<$<AND:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>,$<COMPILE_LANGUAGE:CUDA>>:-G;-src-in-ptx>")
set_target_properties(TestEm3 PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS ON)
