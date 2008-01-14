// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
macros_dir_tlbx_sklt = get_absolute_file_path('buildmacros.sce');
genlib('toolbox_skeletonlib',macros_dir_tlbx_sklt,%f,%t);
// ====================================================================
// clear variables on stack
clear getsave;
clear genlib;
clear cd;
clear toolbox_skeletonlib;
clear macros_dir_tlbx_sklt;
// ====================================================================
