# Install script for directory: /home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.3-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-build/lib/libassimpd.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/anim.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/aabb.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/camera.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/color4.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-build/code/../include/assimp/config.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ColladaMetaData.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/commonMetaData.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/defs.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/light.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/material.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/mesh.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ObjMaterial.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/pbrmaterial.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GltfMaterial.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/scene.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/metadata.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/texture.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/types.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/version.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cimport.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/AssertHandler.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/cexport.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOStream.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/DefaultIOSystem.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ZipArchiveIOSystem.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SceneCombiner.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/fast_atof.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/qnan.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BaseImporter.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Hash.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MemoryIOWrapper.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ParsingUtils.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamReader.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StreamWriter.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringComparison.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StringUtils.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SGSpatialSort.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/GenericProperty.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SpatialSort.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SkeletonMeshBuilder.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmallVector.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/SmoothingGroups.inl"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/StandardShapes.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/RemoveComments.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Subdivision.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Vertex.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LineSplitter.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/TinyFormatter.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Profiler.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/LogAux.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Bitmap.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XMLTools.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/IOStreamBuffer.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/CreateAnimMesh.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/XmlParser.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/BlobIOSystem.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/MathFunctions.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Exceptional.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/ByteSwapper.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/milli/Uni/Cloud_Gaming/ViennaVulkanEngine/cmake-build-debug/_deps/assimp-build/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
