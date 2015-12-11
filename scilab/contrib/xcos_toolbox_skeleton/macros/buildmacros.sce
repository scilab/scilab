// This file is released under the 3-clause BSD license. See COPYING-BSD.

function buildmacros()
    macros_path = get_absolute_file_path("buildmacros.sce");

    blocks = ["TBX_SUM_c", "TBX_SUM_sci", "TBX_SUM_modelica", "TBX_MUT_STYLE", "TBX_NOOP"];

    tbx_build_macros(TOOLBOX_NAME, macros_path);
    tbx_build_blocks(toolbox_dir, blocks);
    tbx_build_pal_loader(TOOLBOX_TITLE, blocks, toolbox_dir, macros_path)
endfunction

buildmacros();
clear buildmacros; // remove buildmacros on stack

