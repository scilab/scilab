// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================

tbx_build_src(['fsum'], ['fsum.f'], 'f', ..
              get_absolute_file_path('builder_fortran.sce'));

clear tbx_build_src;
