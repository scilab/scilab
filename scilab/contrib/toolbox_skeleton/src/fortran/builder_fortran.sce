// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_fortran()

    tbx_build_src(["fsum"],   ..
    ["fsum.f"], ..
    "f",        ..
    get_absolute_file_path("builder_fortran.sce"));

endfunction

builder_fortran();
clear builder_fortran; // remove builder_fortran on stack
