// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
sci_gateway_dir = get_absolute_file_path('loader_gateway.sce');

current_dir = pwd();

chdir(sci_gateway_dir);
if ( isdir('c') ) then
  chdir('c');
  exec('loader.sce');
else
end

chdir(sci_gateway_dir);
if ( isdir('fortran') ) then
  chdir('fortran');
  exec('loader.sce');
else
end

chdir(current_dir);
// ====================================================================
clear sci_gateway_dir;
clear current_dir;
// ====================================================================
