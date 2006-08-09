
// Copyright INRIA
host("nmake /f Makefile.mak");
link('libtdll.dll",'doit','c');
[a,b]=fort('doit',1,1,'d',2,2,'d','sort',1,2);

