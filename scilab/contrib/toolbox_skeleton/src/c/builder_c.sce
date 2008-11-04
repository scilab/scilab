// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================

tbx_build_src(['csum','csub'], ['csum.o','csub.o'], 'c', ..
              get_absolute_file_path('builder_c.sce'));

clear tbx_build_src;
