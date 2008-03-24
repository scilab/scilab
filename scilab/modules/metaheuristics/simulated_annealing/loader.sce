mode(-1)

// Load macros
path = get_absolute_file_path('loader.sce');
disp('Loading macros in ' + path);
if MSDOS then
  load(path+'\macros\lib');
else
  load(path+'/macros/lib');
end
// Load help pages
add_help_chapter('Simulated Annealing',path + 'help');//add help chapter
clear path add_help_chapter get_absolute_file_
