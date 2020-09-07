file(GLOB SRC_FILES "./*.cpp")
file(GLOB HEAD_FILES "./*.h")
file(GLOB RC_FILES "./*rc")
add_executable(VegInfoSystem WIN32 ${SRC_FILES} ${HEAD_FILES} ${RC_FILES})