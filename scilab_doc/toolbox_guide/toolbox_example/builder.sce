mode(-1);
mainpathB=get_absolute_file_path('builder.sce');
chdir(mainpathB);
if isdir('src') then
  chdir('src');
  exec('builder.sce');
  chdir('..');
end
if isdir('interface') then
  chdir('interface');
  exec('builder.sce');
  chdir('..');
end
if isdir('macros') then
  chdir('macros');
  exec('builder.sce');
  chdir('..');
end
if isdir('man') then
  chdir('man');
  exec('builder.sce');
  chdir('..');
end
clear mainpathB