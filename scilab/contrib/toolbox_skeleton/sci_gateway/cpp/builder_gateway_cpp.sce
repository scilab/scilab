// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_cpp()

    tbx_build_gateway("skeleton_cpp", ..
    ["cpp_find","sci_cpp_find"], ..
    ["sci_cpp_find.cxx"], ..
    get_absolute_file_path("builder_gateway_cpp.sce"));

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
