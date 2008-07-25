// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================
sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

tbx_builder_gateway_lang('fortran', sci_gateway_dir);
tbx_builder_gateway_lang('c', sci_gateway_dir);

clear tbx_builder_gateway_lang;
clear sci_gateway_dir;
