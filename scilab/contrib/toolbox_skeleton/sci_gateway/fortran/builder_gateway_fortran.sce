// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_fortran()

    tbx_build_gateway("skeleton_fortran", ..
    ["fortran_sum","sci_fsum"], ..
    ["sci_fsum.c"], ..
    get_absolute_file_path("builder_gateway_fortran.sce"), ..
    ["../../src/fortran/libfsum"]);

endfunction

builder_gw_fortran();
clear builder_gw_fortran; // remove builder_gw_fortran on stack
