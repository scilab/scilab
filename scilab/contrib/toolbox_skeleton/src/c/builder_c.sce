// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================

tbx_build_src(['csum','csub'], ['csum.c','csub.c'], 'c', ..
              get_absolute_file_path('builder_c.sce'));

clear tbx_build_src;
