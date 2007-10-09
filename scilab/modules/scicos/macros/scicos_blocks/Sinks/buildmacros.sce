//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('scssinkslib','SCI/modules/scicos/macros/scicos_blocks/Sinks',%f,%t);
//------------------------------------

