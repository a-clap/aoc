function(pad_string str_out str_in len fill)
    string(LENGTH "${str_in}" strlen)
    math(EXPR pad_size "${len} - ${strlen}")

    set(padded ${str_in})

    foreach (i RANGE 1 ${pad_size})
        string(APPEND padded ${fill})
    endforeach ()

    set(${str_out} "${padded}" PARENT_SCOPE)
endfunction()

function(report_variable var)
    pad_string(str "${var}: " 35 " ")

    string(STRIP "${${var}}" out)
    string(APPEND str "${out}")
    message(STATUS "${str}")
endfunction()
