//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('elementary_functionlib','SCI/modules/elementary_functions/macros',%f,%t);
//------------------------------------
