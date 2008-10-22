//------------------------------------
// Allan CORNET
// Scilab team
// Copyright INRIA
// Date :  Aout 2006
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('scipadlib','SCI/modules/scipad/macros');
genlib('scipadinternalslib','SCI/modules/scipad/macros/scipad_internals',%f,%t);
//------------------------------------
