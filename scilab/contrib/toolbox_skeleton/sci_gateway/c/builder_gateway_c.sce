// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_c()

    includes_src_c = ilib_include_flag(get_absolute_file_path("builder_gateway_c.sce") + "../../src/c");

    // PutLhsVar managed by user in sci_sum and in sci_sub
    // if you do not this variable, PutLhsVar is added
    // in gateway generated (default mode in scilab 4.x and 5.x)
    WITHOUT_AUTO_PUTLHSVAR = %t;

    tbx_build_gateway("skeleton_c", ..
    ["c_sum","sci_csum", "csci"; ...
    "c_sub","sci_csub", "csci"; ...
    "c_multiplybypi","sci_multiplybypi", "csci"; ...
    "foo","sci_foo", "csci"; ...
    "c_error","sci_cerror", "csci"; 
    "c_sum6","sci_csum6", "csci6"; ...
    "c_sub6","sci_csub6", "csci6"; ...
    "c_multiplybypi6","sci_multiplybypi6", "csci6"; ...
    "foo6","sci_foo6", "csci6"; ...
    "c_error6","sci_cerror6", "csci6"], ..
    ["sci_csum.c","sci_csub.c","sci_multiplybypi.c","sci_foo.c","sci_cerror.c", ...
    "sci_csum6.c","sci_csub6.c","sci_multiplybypi6.c","sci_foo6.c","sci_cerror6.c"], ..
    get_absolute_file_path("builder_gateway_c.sce"), ..
    ["../../src/c/libcsum"], ..
    "", ..
    includes_src_c);

endfunction

builder_gw_c();
clear builder_gw_c; // remove builder_gw_c on stack
