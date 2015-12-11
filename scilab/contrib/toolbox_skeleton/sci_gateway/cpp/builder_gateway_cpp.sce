// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_cpp()

    includes_src_c = ilib_include_flag(get_absolute_file_path("builder_gateway_cpp.sce") + "../../src/c");

    tbx_build_gateway("skeleton_cpp", ..
        ["cpp_find","sci_cpp_find", "csci"; ..
        "cpp_error", "sci_cpperror", "cppsci"; ..
        "cpp_foo", "sci_cppfoo", "cppsci"; ..
        "cpp_sum", "sci_cppsum", "cppsci"; ..
        "cpp_sub", "sci_cppsub", "cppsci"; ..
        "cpp_multiplybypi", "sci_cppmultiplybypi", "cppsci"], ..
        ["sci_cpp_find.cxx", "sci_cpperror.cpp", "sci_cppfoo.cpp", "sci_cppsum.cpp", "sci_cppsub.cpp", "sci_cppmultiplybypi.cpp"], ..
        get_absolute_file_path("builder_gateway_cpp.sce"), ..
        ["../../src/c/libcsum"], ..
        "", ..
        includes_src_c);

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
