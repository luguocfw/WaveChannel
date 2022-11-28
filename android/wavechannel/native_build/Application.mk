STLPORT_FORCE_REBUILD := false
APP_STL := c++_shared
APP_CPPFLAGS += -fexceptions  -std=c++11
APP_CPPFLAGS += -frtti
APP_ABI := armeabi-v7a arm64-v8a
GLOBAL_CFLAGS += -O0 -std=c++11 -Werror -Wno-deprecated-declarations -Wno-switch
APP_PLATFORM := android-21
APP_SHORT_COMMANDS := true

ATOM_COMPILER_OPT := -march=atom -mno-movbe -msse3 -mfpmath=sse -ffast-math -mstackrealign