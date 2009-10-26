// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================

src_c_path = get_absolute_file_path('builder_c.sce');

CFLAGS = "-I" + src_c_path;

tbx_build_src(['csum','csub'], ['csum.c','csub.c'], 'c', ..
              src_c_path, '', '', CFLAGS);

clear tbx_build_src;
clear src_c_path;
clear CFLAGS;
