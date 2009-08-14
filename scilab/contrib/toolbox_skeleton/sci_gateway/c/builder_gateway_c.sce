// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================

if MSDOS then
  // to manage long pathname
  includes_src_c = '-I""' + get_absolute_file_path('builder_gateway_c.sce') + '../../src/c""';
else
  includes_src_c = '-I' + get_absolute_file_path('builder_gateway_c.sce') + '../../src/c';
end

tbx_build_gateway('skeleton_c', ['c_sum','sci_csum';'c_sub','sci_csub'], ['sci_csum.c','sci_csub.c'], ..
                  get_absolute_file_path('builder_gateway_c.sce'), ..
                  ['../../src/c/libcsum'],'',includes_src_c);

clear tbx_build_gateway;
