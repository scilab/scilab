
// Copyright INRIA
if (findmsvccompiler()<>'unknown') then
  dos("nmake /f Makefile.mak");
  link('libtdll.dll",'doit','c');
  [a,b]=call('doit',1,1,'d',2,2,'d','sort',1,2);
end  


