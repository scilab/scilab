// This file is released under the 3-clause BSD license. See COPYING-BSD.

if getos() == "Windows" then
    // to manage long pathname
    includes_src_c = "-I""" + get_absolute_file_path("builder_gateway_c.sce") + "../../src/c""";
else
    includes_src_c = "-I" + get_absolute_file_path("builder_gateway_c.sce") + "../../src/c";
end

tbx_build_gateway("xcos_toolbox_skeleton_c",                        ..
                  ["tbx_sum", "sci_tbx_sum"],                       ..
                  ["sci_tbx_sum.c"],                                ..
                  get_absolute_file_path("builder_gateway_c.sce"),  ..
                  ["../../src/c/libxcos_tbx_skel"],                 ..
                  "",                                               ..
                  includes_src_c);

clear tbx_build_gateway;

