// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================
sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

tbx_builder_gateway_lang('fortran', sci_gateway_dir);
tbx_builder_gateway_lang('c', sci_gateway_dir);
tbx_builder_gateway_lang('cpp', sci_gateway_dir);
tbx_build_gateway_loader(['c', 'fortran','cpp'], sci_gateway_dir);

clear tbx_builder_gateway_lang tbx_build_gateway_loader;
clear sci_gateway_dir;
