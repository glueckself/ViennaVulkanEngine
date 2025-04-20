# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-src")
  file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-src")
endif()
file(MAKE_DIRECTORY
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-build"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/tmp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/src/l2dfiledialog-populate-stamp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/src"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/src/l2dfiledialog-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/src/l2dfiledialog-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/l2dfiledialog-subbuild/l2dfiledialog-populate-prefix/src/l2dfiledialog-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
