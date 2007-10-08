//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('scselectricallib','SCI/modules/scicos/macros/scicos_blocks/Electrical');
//------------------------------------
if MSDOS then
  unix("dir /B *.mo >models");
else
  unix("ls *.mo >models");
end
if with_modelicac() then exec("../../genmoc.sce");end;
//------------------------------------
