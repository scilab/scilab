// ------------------------------------
// Allan CORNET
// INRIA 2006
// ------------------------------------
//warning('off'); // disable warnings
// ------------------------------------
getf(SCI+'/modules/fileio/macros/fileparts.sci');
getf(SCI+'/modules/core/macros/getshell.sci');
getf(SCI+'/modules/fileio/macros/pathconvert.sci');
getf(SCI+'/modules/string/macros/stripblanks.sci');
getf(SCI+'/modules/fileio/macros/listfiles.sci');
getf(SCI+'/modules/io/macros/unix_g.sci');
getf(SCI+'/modules/core/macros/OS_Version.sci');
getf(SCI+'/modules/fileio/macros/isdir.sci');
getf(SCI+'/modules/overloading/macros/%c_a_c.sci');
getf(SCI+'/modules/fileio/macros/basename.sci');
getf(SCI+'/modules/fileio/macros/mputl.sci');
getf(SCI+'/modules/functions/macros/genlib.sci');
// ------------------------------------
lines(0);
Directories=[];
CurrentDirectory=pwd();

modules=getmodules();
index=size(modules);

for i=1:index(1) do 
 Directories=[Directories;"SCI/modules/"+modules(i)+"/macros"];
end  
clear modules index i
 
Dim=size(Directories);


for i=1:Dim(1) do 
  chdir(Directories(i));
  mprintf(" -- Creation of [%s] (Macros) --\n",Directories(i));
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
// ------------------------------------
