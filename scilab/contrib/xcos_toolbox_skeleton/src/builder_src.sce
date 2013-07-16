// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_src()
    src_path = get_absolute_file_path("builder_src.sce");
    tbx_builder_src_lang("c", src_path);
endfunction

builder_src();
clear builder_src; // remove builder_src on stack


