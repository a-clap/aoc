set(sanitizers)

if (WITH_ASAN)
    list(APPEND sanitizers "address")
endif ()

if (WITH_UBSAN)
    list(APPEND sanitizers "undefined")
endif ()

if (WITH_LEAKSAN)
    list(APPEND sanitizers "leak")
endif ()

if (WITH_TSAN)
    if (WITH_ASAN OR WITH_TSAN OR WITH_LEAKSAN)
        message(FATAL_ERROR "TSAN doesn't work with ASAN")
    endif ()

    list(APPEND sanitizers "thread")
endif ()

if (WITH_MSAN)
    if (WITH_ASAN OR WITH_TSAN OR WITH_LEAKSAN)
        message(FATAL_ERROR "MSAN doesn't work with other sanitizers")
    endif ()
    list(APPEND sanitizers "memory")
endif ()

list(JOIN sanitizers "," sanitizers)

if (sanitizers)
    set(sanitizers "-fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=${sanitizers}")
endif ()
