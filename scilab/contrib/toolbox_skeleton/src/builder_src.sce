// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
src_dir = get_absolute_file_path('builder_src.sce');

previous_dir = pwd();
cd(src_dir);

if ( isdir('fortran') ) then 
 chdir('fortran');
 exec('builder_fortran.sce');
else
end

cd(src_dir);

if ( isdir('c') ) then 
 chdir('c');
 exec('builder_c.sce');
else
end

cd(previous_dir);
// ====================================================================
clear previous_dir;
clear src_dir;
// ====================================================================
