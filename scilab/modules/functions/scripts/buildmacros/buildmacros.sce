// ------------------------------------
// Allan CORNET
// INRIA 2006
// ------------------------------------
//warning('off'); // disable warnings
// ------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
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
  write(%io(2),'-- Creation of ['+Directories(i)+'] (Macros) --");
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
// ------------------------------------
