// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================

build_gateway('skeleton_fortran', ['fortran_sum','sci_fsum'], ['sci_fsum.o'], ..
              get_absolute_file_path('builder_gateway_fortran.sce'), ..
              ['../../src/fortran/libfsum']);

clear build_gateway;
