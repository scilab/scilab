// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// ====================================================================

build_src(['fsum'], ['fsum.o'], 'f', ..
	get_absolute_file_path('builder_fortran.sce'));

clear build_src;
