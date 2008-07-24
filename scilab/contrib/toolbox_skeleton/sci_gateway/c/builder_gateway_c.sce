// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================

build_gateway('skeleton_c', ['c_sum','sci_csum'], ['sci_csum.o'], ..
              get_absolute_file_path('builder_gateway_c.sce'), ..
	      ['../../src/c/libcsum']);

clear build_gateway;
