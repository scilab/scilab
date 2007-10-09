//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('scsbranchinglib','SCI/modules/scicos/macros/scicos_blocks/Branching',%f,%t);
//------------------------------------
