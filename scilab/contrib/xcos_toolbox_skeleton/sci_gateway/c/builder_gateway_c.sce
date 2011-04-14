// This file is released under the 3-clause BSD license. See COPYING-BSD.

includes_src_c = ilib_include_flag(get_absolute_file_path("builder_gateway_c.sce") + "../../src/c");

tbx_build_gateway("xcos_toolbox_skeleton_c",                        ..
                  ["tbx_sum", "sci_tbx_sum"],                       ..
                  ["sci_tbx_sum.c"],                                ..
                  get_absolute_file_path("builder_gateway_c.sce"),  ..
                  ["../../src/c/libxcos_tbx_skel"],                 ..
                  "",                                               ..
                  includes_src_c);

clear tbx_build_gateway;

