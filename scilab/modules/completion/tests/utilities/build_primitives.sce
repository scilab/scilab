// ====================================================================
// Allan CORNET
// DIGITEO 2009

// ====================================================================
sci_completeline_dir = get_absolute_file_path('build_primitives.sce');

previous_dir = pwd();
cd(sci_completeline_dir);

ilib_name  = 'completion_c';
files = ['sci_completeline.c', 'sci_getpartlevel.c', 'sci_getfilepartlevel.c','sci_getcommonpart.c'];
if MSDOS then
libs = SCI +'/bin/scicompletion';
cflags = "-I " + SCI +"/modules/completion/includes";
else
libs = "";
cflags = "";
end

// name known in scilab , C function called
table = [ 'completeline','sci_completeline';
'getpartlevel','sci_getpartlevel';
'getfilepartlevel','sci_getfilepartlevel';
'getcommonpart','sci_getcommonpart'];

libname = ilib_build(ilib_name,table,files,libs,"makelib","",cflags);

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
