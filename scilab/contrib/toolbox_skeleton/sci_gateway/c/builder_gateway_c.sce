// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
gateway_c_dir = get_absolute_file_path('builder_gateway_c.sce');

previous_dir = pwd();
cd(gateway_c_dir);

ilib_name  = 'skeleton_c'; 		
files = ['sci_csum.o'];    
libs = ['../../src/c/libcsum'];					

// name known in scilab , C function called
table = [ 'c_sum','sci_csum';];

libname = ilib_build(ilib_name,table,files,libs);

cd(previous_dir);
// ====================================================================
clear ilib_build;
clear libname;
clear table;
clear libs;
clear files;
clear ilib_name;
clear previous_dir
clear gateway_c_dir;
// ====================================================================
