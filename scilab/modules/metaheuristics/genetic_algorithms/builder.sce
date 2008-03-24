mode(-1) //force silent execution

// Build macros
toolboxname='GA';
path = get_absolute_file_path('builder.sce');
disp('Building macros  in ' + path + 'macros');
genlib(toolboxname + 'lib', path+'/macros', %T);
clear genlib toolboxname

// Build help pages
add_help_chapter('Genetic Algorithms', path + 'help');//add help chapter
xmltohtml(path + 'help','Genetic Algorithms')
//clear the variable stack
clear path add_help_chapter get_absolute_file_path 
