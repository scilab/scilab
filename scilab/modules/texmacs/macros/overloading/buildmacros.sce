//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('texmacsoverloadinglib','SCI/modules/texmacs/macros/overloading',%f,%t);
//------------------------------------
