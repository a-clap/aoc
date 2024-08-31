include(${CMAKE_CURRENT_LIST_DIR}/warnings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sanitizers.cmake)

set(CMAKE_C_FLAGS_INIT "${warnings}" CACHE INTERNAL "c flags init")
set(CMAKE_CXX_FLAGS_INIT "${warnings}" CACHE INTERNAL "cxx flags init")

set(CMAKE_C_FLAGS_DEBUG_INIT "-g3 -O0 ${sanitizers}" CACHE INTERNAL "c flags debug init")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g3 -O0 ${sanitizers}" CACHE INTERNAL "cxx flags debug init")
SET(CMAKE_ASM_FLAGS_DEBUG_INIT "-g3" CACHE INTERNAL "asm debug compiler flags")

set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 " CACHE INTERNAL "c flags release init")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3" CACHE INTERNAL "cxx flags release init")
set(CMAKE_ASM_FLAGS_RELEASE_INIT "" CACHE INTERNAL "asm release compiler flags")
