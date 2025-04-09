set(TARGET muegame)
set(SOURCE game.cpp store.cpp)
set(HEADERS store.h)

add_executable(${TARGET} ${SOURCE} ${HEADERS})

target_compile_features(${TARGET} PUBLIC cxx_std_20)

target_link_libraries (${TARGET} PUBLIC viennavulkanengine)

