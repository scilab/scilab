mode(-1)
toolboxname='mytoolbox'
pathB=get_absolute_file_path('buildmacros.sce')
disp('Building macros  in ' +pathB)
genlib(toolboxname+'lib',pathB,%t)
clear pathB genlib toolboxname