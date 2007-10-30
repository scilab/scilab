//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('development_toolslib','SCI/modules/development_tools/macros',%f,%t);
exit;
//------------------------------------
