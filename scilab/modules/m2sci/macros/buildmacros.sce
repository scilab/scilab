//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('m2scilib','SCI/modules/m2sci/macros');
//------------------------------------
mprintf("-- Creation of [m2sci] (Macros) --\n");
//------------------------------------
lines(0);
CurrentDirectorym2sci=pwd();
 Directoriesm2sci=["kernel",
 "percent",
 "sci_files"];

stacksize(5000000);

Dim=size(Directoriesm2sci);
for i=1:Dim(1) do 
  chdir(Directoriesm2sci(i));
  exec('buildmacros.sce');
  chdir(CurrentDirectorym2sci);
end

clear CurrentDirectorym2sci Directoriesm2sci Dim
//------------------------------------
