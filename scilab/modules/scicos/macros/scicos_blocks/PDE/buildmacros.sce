//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('PDElib','SCI/modules/scicos/macros/scicos_blocks/PDE',%f,%t);
//------------------------------------