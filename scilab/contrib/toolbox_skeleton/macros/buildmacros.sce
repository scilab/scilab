// This file is released under the 3-clause BSD license. See COPYING-BSD.

function buildmacros()
    macros_path = get_absolute_file_path("buildmacros.sce");
    tbx_build_macros(TOOLBOX_NAME, macros_path);
endfunction

buildmacros();
clear buildmacros; // remove buildmacros on stack

