// This file is released under the 3-clause BSD license. See COPYING-BSD.

// This macro compiles the files

function builder_c()
    src_c_path = get_absolute_file_path("builder_c.sce");

    CFLAGS = ilib_include_flag(src_c_path);
    LDFLAGS = "";
    if (getos()<>"Windows") then
        if ~isdir(SCI+"/../../share") then
            // Source version
            CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos_blocks/includes" ;
            CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos/includes" ;
        else
            // Release version
            CFLAGS = CFLAGS + " -I" + SCI + "/../../include/scilab/scicos_blocks";
            CFLAGS = CFLAGS + " -I" + SCI + "/../../include/scilab/scicos";
        end
    else
        CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos_blocks/includes";
        CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos/includes";

        // Getting symbols
        if findmsvccompiler() <> "unknown" & haveacompiler() then
            LDFLAGS = LDFLAGS + " """ + SCI + "/bin/scicos.lib""";
            LDFLAGS = LDFLAGS + " """ + SCI + "/bin/scicos_f.lib""";
        end
    end

    tbx_build_src(["block_sum", "business_sum", "tbx_block_noop"],          ..
    ["block_sum.c", "business_sum.c" "tbx_block_noop.c"],                   ..
    "c",                                                                    ..
    src_c_path,                                                             ..
    "",                                                                     ..
    LDFLAGS,                                                                ..
    CFLAGS,                                                                 ..
    "",                                                                     ..
    "",                                                                     ..
    "xcos_tbx_skel");
endfunction

builder_c();
clear builder_c; // remove builder_c on stack
