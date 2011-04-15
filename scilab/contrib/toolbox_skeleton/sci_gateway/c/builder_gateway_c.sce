// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_c()

  includes_src_c = ilib_include_flag(get_absolute_file_path("builder_gateway_c.sce") + "../../src/c");

  // PutLhsVar managed by user in sci_sum and in sci_sub
  // if you do not this variable, PutLhsVar is added
  // in gateway generated (default mode in scilab 4.x and 5.x)
  WITHOUT_AUTO_PUTLHSVAR = %t;

  tbx_build_gateway("skeleton_c", ..
                    ["c_sum","sci_csum";"c_sub","sci_csub";"c_multiplybypi","sci_multiplybypi"], ..
                    ["sci_csum.c","sci_csub.c","sci_multiplybypi.c"], ..
                    get_absolute_file_path("builder_gateway_c.sce"), ..
                    ["../../src/c/libcsum"], ..
                    "", ..
                   includes_src_c);
                   
endfunction

builder_gw_c();
clear builder_gw_c; // remove builder_gw_c on stack
