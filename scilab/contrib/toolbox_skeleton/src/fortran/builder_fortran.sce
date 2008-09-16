// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================

tbx_build_src(['fsum'], ['fsum.f'], 'f', ..
              get_absolute_file_path('builder_fortran.sce'));

clear tbx_build_src;
