// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gateway()

    sci_gateway_dir = get_absolute_file_path("builder_gateway.sce");
    languages       = ["c" "fortran" "cpp"];

    tbx_builder_gateway_lang(languages,sci_gateway_dir);
    tbx_build_gateway_loader(languages,sci_gateway_dir);
    tbx_build_gateway_clean(languages,sci_gateway_dir);

endfunction

builder_gateway()
clear builder_gateway; // remove builder_gateway on stack
