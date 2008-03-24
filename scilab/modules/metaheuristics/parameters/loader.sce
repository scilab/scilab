mode(-1)

// Load macros
path = get_absolute_file_path('loader.sce');
disp('Loading macros  in ' + path);
if MSDOS then
  load(path+'\macros\lib');
else
  load(path+'/macros/lib');
end
// Load help pages
add_help_chapter('Params',path + 'help');//add help chapter
clear path
