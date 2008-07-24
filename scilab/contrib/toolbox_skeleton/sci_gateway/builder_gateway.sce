// ====================================================================
// Allan CORNET
// INRIA 2008
// ====================================================================
sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

builder_gateway_lang('fortran', sci_gateway_dir);
builder_gateway_lang('c', sci_gateway_dir);

clear builder_gateway_lang;
clear sci_gateway_dir;
