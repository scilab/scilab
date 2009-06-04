// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================

tbx_build_gateway('libintfreefem', ['freefemi','freefemi'], ['freefemi.f'], ..
                  get_absolute_file_path('builder_gateway_fortran.sce'), ..
                  ['../../src/c/libfreefem]);

clear tbx_build_gateway;
