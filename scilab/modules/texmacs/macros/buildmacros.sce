//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('texmacslib','SCI/modules/texmacs/macros',%f,%t);
//------------------------------------
lines(0);
CurrentDirectorytexmacs = get_absolute_file_path("buildmacros.sce");
macrosdirs  = ["overloading"];

for i=1:size(macrosdirs,"*") do
  exec(CurrentDirectorytexmacs+"/"+macrosdirs(i)+"/buildmacros.sce");
end

clear my_script_path CurrentDirectorytexmacs macrosdirs
//------------------------------------