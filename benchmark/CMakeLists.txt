# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

link_directories(${CMAKE_CURRENT_BINARY_DIR}
                 /usr/local/lib)

include_directories (${PROJECT_SOURCE_DIR}/src
                     ${PROJECT_SOURCE_DIR}/include
                     ${PROJECT_SOURCE_DIR}/benchmark
                     ${PROJECT_BINARY_DIR}/include
                     /usr/local/include)

configure_file(
  "bench.c.in"
  "${CMAKE_CURRENT_BINARY_DIR}/bench.c"
)

add_library(mpc_bench_utils OBJECT ${CMAKE_CURRENT_BINARY_DIR}/bench.c)

# List of benchmarks
file(GLOB_RECURSE SRCS *.c)

foreach(bench ${SRCS})
  # Extract the filename without an extension
  get_filename_component(target ${bench} NAME_WE)

  add_executable(${target} ${bench} $<TARGET_OBJECTS:mpc_bench_utils>)

  target_link_libraries(${target} amcl_mpc)
endforeach(bench)
