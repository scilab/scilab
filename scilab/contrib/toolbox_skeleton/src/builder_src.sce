// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_src()
    langage_src = ["fortran" "c" "java"];
    path_src = get_absolute_file_path("builder_src.sce");
    tbx_builder_src_lang(langage_src, path_src);
endfunction

builder_src();
clear builder_src; // remove builder_src on stack
