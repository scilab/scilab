mode(-1);
mainpathL=get_absolute_file_path('loader.sce');
chdir(mainpathL);
if isdir('sci_gateway') then
chdir('sci_gateway');
exec('loader.sce');
chdir('..');
end
if isdir('macros') then
chdir('macros');
exec('loadmacros.sce');
chdir('..');
end
if isdir('help') then
chdir('help');
exec('loadhelp.sce');
chdir('..');
end
clear mainpathL