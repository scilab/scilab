// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
gateway_fortran_dir = get_absolute_file_path('builder_gateway_fortran.sce');

previous_dir = pwd();
cd(gateway_fortran_dir);

ilib_name  = 'skeleton_fortran'; 		
files = ['sci_fsum.o'] ;   
libs = ['../../src/fortran/libfsum'];					

// name known in scilab , C function called
table = [ 'fortran_sum','sci_fsum';];

libname = ilib_build(ilib_name,table,files,libs);

cd(previous_dir);
// ====================================================================
clear ilib_build;
clear libname;
clear table;
clear libs;
clear files;
clear ilib_name;
clear previous_dir;
clear gateway_fortran_dir;
// ====================================================================
