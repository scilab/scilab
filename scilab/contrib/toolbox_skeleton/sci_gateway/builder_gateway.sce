// This file is released into the public domain

sci_gateway_dir = get_absolute_file_path("builder_gateway.sce");
languages       = ["fortran" "c" "cpp"];

tbx_builder_gateway_lang(languages,sci_gateway_dir);
tbx_build_gateway_loader(languages,sci_gateway_dir);
tbx_build_gateway_clean(languages,sci_gateway_dir);

clear tbx_builder_gateway_lang tbx_build_gateway_loader;
clear sci_gateway_dir languages;
