# SPDX-FileCopyrightText: 2020 CERN
# SPDX-License-Identifier: Apache-2.0

# Example 1 of particle processing with GPU
add_executable(example1 example1.cu)
target_link_libraries(example1 PRIVATE AdePT VecGeom::vecgeom VecGeom::vecgeomcuda_static VecGeom::vgdml)
set_target_properties(example1 PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS ON)
add_test(NAME example1 COMMAND example1)

