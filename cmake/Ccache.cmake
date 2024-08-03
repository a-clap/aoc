# Find ccache
find_program(CACHE NAMES "ccache" REQUIRED)

set(CMAKE_CXX_COMPILER_LAUNCHER ${CACHE} CACHE FILEPATH "CXX compiler cache used")
set(CMAKE_C_COMPILER_LAUNCHER ${CACHE} CACHE FILEPATH "C compiler cache used")
