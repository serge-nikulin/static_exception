# Copyright 2018 Apex.AI, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_package(OpenMP REQUIRED)
add_executable(parallel_test
  parallel_test_static_exception.cpp
  ../include/exception_memory_pool.hpp
  ../include/unwind-cxx.h)

if(WIN32)
  add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
  target_compile_options(parallel_test PRIVATE "/openmp" "/Oi" "/arch:AVX2" "/Oy")
else()
  target_compile_options(parallel_test PRIVATE "--std=c++11" "-O3")
  target_link_libraries(parallel_test PRIVATE "${OpenMP_CXX_FLAGS}")
  target_compile_options(parallel_test PRIVATE "${OpenMP_CXX_FLAGS}")
endif()
