function libn=ilib_compile(lib_name,makename)
// Copyright ENPC
// call make for target files or objects depending
// on OS and compilers
// very similar to G_make
//-------------------------------------------------
if typeof(lib_name)<>'string' then
  error('ilib_compile: first argument must be a string');
  return ;
end
oldpath=getcwd();
k=strindex(makename,['/','\'])
if k~=[] then
  path=part(makename,1:k($))
  makename=part(makename,k($)+1:length(makename))
  chdir(path)
else
  path=''
end
comp_target = getenv('COMPILER','NO');
if getenv('WIN32','NO')=='OK' then
  libn=lib_name+'.dll'
  select comp_target
  case 'VC++' then unix_s('nmake /f '+ makename +'.mak '+libn);
  case 'ABSOFT' then unix_s('amake /f '+ makename +'.amk '+libn);
  else // gnuwin32 ?
    unix_s('make -f ' + makename+' '+libn);
  end
else
  unix_s('make -f '+ makename+' '+lib_name+'.la');
  libn=lib_name+'.so'
end
libn=path+libn
chdir(oldpath)





