//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
if %pvm then
genlib('pvmlib','SCI/modules/pvm/macros');
end
//------------------------------------
