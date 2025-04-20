# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-src")
  file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-src")
endif()
file(MAKE_DIRECTORY
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-build"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/tmp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/src/viennaentitycomponentsystem-populate-stamp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/src"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/src/viennaentitycomponentsystem-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/src/viennaentitycomponentsystem-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/viennaentitycomponentsystem-subbuild/viennaentitycomponentsystem-populate-prefix/src/viennaentitycomponentsystem-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
