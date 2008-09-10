// ====================================================================
// Allan CORNET
// DIGITEO 2008
// ====================================================================

tbx_build_gateway('skeleton_cpp', ['cpp_find','sci_cpp_find'], ['sci_cpp_find.cxx'], ..
                  get_absolute_file_path('builder_gateway_cpp.sce'));

clear tbx_build_gateway;
