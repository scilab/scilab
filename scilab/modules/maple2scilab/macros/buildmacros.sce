//------------------------------------
// Allan CORNET
// Scilab team
// Copyright INRIA
// Date : Aout 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('maple2scilablib','SCI/modules/maple2scilab/macros');
//------------------------------------
