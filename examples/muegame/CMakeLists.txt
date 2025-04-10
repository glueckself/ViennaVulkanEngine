set(TARGET muegame)
set(SOURCE game.cpp store.cpp)
set(HEADERS store.h)

add_executable(${TARGET} ${SOURCE} ${HEADERS})

target_compile_features(${TARGET} PUBLIC cxx_std_20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

target_link_libraries (${TARGET} PUBLIC viennavulkanengine PkgConfig::LIBAV)

