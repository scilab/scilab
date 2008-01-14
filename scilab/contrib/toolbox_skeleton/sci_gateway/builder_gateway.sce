// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================
sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

previous_dir = pwd();

cd(sci_gateway_dir);
if ( isdir('c') ) then
  chdir('c');
  exec('builder_gateway_c.sce');
else
end

cd(sci_gateway_dir);
if ( isdir('fortran') ) then
  chdir('fortran');
  exec('builder_gateway_fortran.sce');
else
end
// ====================================================================
clear cd;
clear previous_dir;
clear sci_gateway_dir;
// ====================================================================
