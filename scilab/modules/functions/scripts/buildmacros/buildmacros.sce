// ------------------------------------
// Allan CORNET
// INRIA 2006
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

exec('SCI/modules/functions/scripts/buildmacros/genmacros.sce');

for i=1:Dim(1) do 
  chdir(Directories(i));
  disp('-- Creation of ['+Directories(i)+'] (Macros) --');
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
// ------------------------------------
