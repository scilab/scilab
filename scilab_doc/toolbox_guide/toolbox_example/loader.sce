mode(-1);
mainpathL=get_absolute_file_path('loader.sce');
chdir(mainpathL);
if isdir('interface') then
  chdir('interface');
  exec('loader.sce');
  chdir('..');
end
if isdir('macros') then
  chdir('macros');
  exec('loader.sce');
  chdir('..');
end
if isdir('man') then
  chdir('man');
  exec('loader.sce');
  chdir('..');
end
clear mainpathL