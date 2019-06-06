project(catch2_download)

if (TARGET catch2)
  return()
endif()

include(ExternalProject)
ExternalProject_Add(catch2_external GIT_REPOSITORY https://github.com/catchorg/Catch2.git)

ExternalProject_Get_Property(catch2_external source_dir)
set(CATCH2_INCLUDE_DIR ${source_dir}/include)

# Library
add_library(catch2 INTERFACE)
add_dependencies(catch2 catch2_external)
target_include_directories(catch2 INTERFACE ${source_dir}/include)
