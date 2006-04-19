mode(-1)
toolboxname='toolbox_example'
pathB=get_absolute_file_path('builder.sce')
disp('Building macros  in ' +pathB)
genlib(toolboxname+'lib',pathB,%t)
clear pathB genlib toolboxname