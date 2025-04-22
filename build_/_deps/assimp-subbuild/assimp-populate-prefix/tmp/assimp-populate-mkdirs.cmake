# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-src"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-build"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/tmp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/src/assimp-populate-stamp"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/src"
  "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/src/assimp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/src/assimp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/build_/_deps/assimp-subbuild/assimp-populate-prefix/src/assimp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
