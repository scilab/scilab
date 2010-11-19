// This file is released under the 3-clause BSD license. See COPYING-BSD.

tbx_build_gateway("skeleton_fortran", ..
                  ["fortran_sum","sci_fsum"], ..
                  ["sci_fsum.c"], ..
                  get_absolute_file_path("builder_gateway_fortran.sce"), ..
                  ["../../src/fortran/libfsum"]);

clear tbx_build_gateway;
